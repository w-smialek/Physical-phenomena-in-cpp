#include <random>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

class Virus
{
    public:

        double repMaxProb;
        double deactProb;

        virtual bool doesDeact(vector<string> treatments = {}) = 0;
        virtual Virus* Reproduce(double dens) = 0;

        mt19937 gen = mt19937((random_device())());
        uniform_real_distribution<double> uniform = uniform_real_distribution<double>(0,1);
};

class SimpleVirus
    : public Virus
{
    public:

        SimpleVirus()
        {
            repMaxProb = 0.1;
            deactProb = 0.1;
        }

        SimpleVirus(double rep, double deact)
        {
            repMaxProb = rep;
            deactProb = deact;
        }

        bool doesDeact(vector<string> treatments = {})
        {
            double toss = uniform(gen);
            if (toss<deactProb)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        Virus* Reproduce(double dens)
        {
            double toss = uniform(gen);
            if (toss<repMaxProb*(1-dens))
            {
                Virus* nv = new SimpleVirus(repMaxProb, deactProb);
                return nv;
            }
            else
            {
                return nullptr;
            }
        }
};

class ResistVirus
    : public Virus
{
    public:

        double mutProb;
        map<string, bool> existent_treatments;

        ResistVirus()
        {
            repMaxProb = 0.1;
            deactProb = 0.1;
            mutProb = 0.01;
        }

        ResistVirus(double rep, double deact, double mut, map<string, bool> trts)
        {
            repMaxProb = rep;
            deactProb = deact;
            mutProb = mut;
            existent_treatments = trts;
        }

        bool doesDeact(vector<string> treatments = {})
        {
            std::map<string, bool>::iterator ite;

            for(int a = 0; a < treatments.size(); a++)
            {
                ite = existent_treatments.find(treatments[a]);
                if (ite->second == false)
                {
                    return true;
                }
            }

            double toss = uniform(gen);
            if (toss<deactProb)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    
        Virus* Reproduce(double dens)
        {
            double toss = uniform(gen);
            if (toss<repMaxProb*(1-dens))
            {
                map<string, bool> mutated_trts = existent_treatments;
                map<string, bool>::iterator itd;
                for (itd = mutated_trts.begin(); itd != mutated_trts.end(); itd++)
                {
                    toss = uniform(gen);
                    if (toss < mutProb)
                    {
                        itd -> second = not itd -> second;
                    }
                }
                Virus* nv = new ResistVirus(repMaxProb, deactProb, mutProb, mutated_trts);
                return nv;
            }

            else
            {
                return nullptr;
            }
        }
};

class Patient
{
    public:

    Patient()
    {
        maxVir = 100;
    }

    Patient(int max)
    {
        maxVir = max;
    }
    
    int maxVir;

    vector<string> presentTrts;

    vector<Virus*> Svirs;
    vector<Virus*> Rvirs;

    void update(bool resist)
    {
        int nvirs;
        double dvirs;

        if(resist)
        {
            dvirs = Rvirs.size();
            dvirs /= maxVir;
            cout<<dvirs<<endl;

            nvirs = Rvirs.size();

            for (int ix = 0; ix < nvirs ; ix++)
            {
                if((*Rvirs[ix]).doesDeact(presentTrts))
                {
                    delete Rvirs[ix];
                    Rvirs.erase(Rvirs.begin() + ix);
                    ix--;
                    nvirs--;
                    continue;
                }

                Virus* new_virus_ptr = (*Rvirs[ix]).Reproduce(dvirs);

                if (new_virus_ptr != nullptr)
                {
                    Rvirs.push_back(new_virus_ptr);
                }

            }        
        }
        else
        {
            dvirs = Svirs.size();
            dvirs /= maxVir;
            cout<<dvirs<<endl;

            nvirs = Svirs.size();

            for (int ix = 0; ix < nvirs ; ix++)
            {
                if((*Svirs[ix]).doesDeact(presentTrts))
                {
                    delete Svirs[ix];
                    Svirs.erase(Svirs.begin() + ix);
                    ix--;
                    nvirs--;
                    continue;
                }

                Virus* new_virus_ptr = (*Svirs[ix]).Reproduce(dvirs);

                if (new_virus_ptr != nullptr)
                {
                    Svirs.push_back(new_virus_ptr);
                }

            }        
        }
    }

    void SInfect(double rep, double deact)
    {
        Virus* nv = new SimpleVirus(rep, deact);
        Svirs.push_back(nv);
    }
    void RInfect(double rep, double deact, double mut, map<string, bool> trts)
    {
        Virus* nv = new ResistVirus(rep, deact, mut, trts);
        Rvirs.push_back(nv);
    }

    void NewTreatment(string trt)
    {
        presentTrts.push_back(trt);
    }
    void RemoveTreatment(string trt)
    {
        std::vector<string>::iterator itst;
        itst = find (presentTrts.begin(), presentTrts.end(), trt);

        if (itst != presentTrts.end())
        {
            presentTrts.erase(itst);
        }
        else
        {
            // coś się może dziać
        }
    }

};


int main()
{
    map<string, bool> existent_trts;
    existent_trts.insert(pair<string, bool>("medyczna marihuana", false));
    existent_trts.insert(pair<string, bool>("dmt", true));
    existent_trts.insert(pair<string, bool>("amantadyna", false));

    Patient p0(100);
    p0.NewTreatment("dmt");
    p0.SInfect(0.2,0.03);
    p0.RInfect(0.2, 0.01, 0.05, existent_trts);
    for(int ika = 0; ika < 100; ika++)
    {
        p0.update(false);
        cout<<"rozm "<<p0.Rvirs.size()<<endl;
    }
    p0.NewTreatment("amantadyna");
        for(int ika = 0; ika < 100; ika++)
    {
        p0.update(true);
        cout<<"rozm "<<p0.Rvirs.size()<<endl;
    }
    return 0;
}