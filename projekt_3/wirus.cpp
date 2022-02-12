#include <random>
#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
using namespace std;

void print_strvector(vector<string> vec)
{
for (auto i: vec) {
    std::cout << i << endl; // will print: "a b c"
}
}

class Virus
{
    public:

        double repMaxProb;
        double deactProb;

        mt19937 gen = mt19937((random_device())());
        normal_distribution<double> normal = normal_distribution<double>(0, 2700);
        uniform_real_distribution<double> uniform = uniform_real_distribution<double>(0,1);

        // Virus* Reproduce(double popDens);
        // bool doesDeact();
};

class SimpleVirus
    : public Virus
{
    public:

        SimpleVirus()
        {
            repMaxProb = 0;
            deactProb = 0;
        }

        SimpleVirus(double rep, double deact)
        {
            repMaxProb = rep;
            deactProb = deact;
        }

        bool doesDeact()
        {
            double toss = uniform(gen);
            if (toss < deactProb)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        SimpleVirus* Reproduce(double popDens, SimpleVirus* ptrtovrs)
        {
            double toss = uniform(gen);
            if (toss < repMaxProb * (1 - popDens))
            {
                SimpleVirus new_virus = SimpleVirus(repMaxProb, deactProb);
                ptrtovrs = &new_virus;
                return ptrtovrs;
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

        map<string, bool> treatments;

       ResistVirus()
        {
            repMaxProb = 0;
            deactProb = 0;
            mutProb = 0;
            // treatments =  tutaj ogarnąć w ooogóle te mapy i treatmenty
        }
        ResistVirus(double rep, double deact, double mut, map<string, bool> trt)
        {
            repMaxProb = rep;
            deactProb = deact;
            mutProb = mut;
            treatments = trt;
        }

        bool doesDeact(vector<string> treatment)
        {
            map<string, bool>::iterator itr;
            
            for (int i = 0; i < treatment.size(); i++)
            {
                for (itr = treatments.begin(); itr != treatments.end(); ++itr) 
                {
                    if (itr->first == treatment[i])
                    {
                        if (not itr->second)
                        {
                            return true;
                        }
                    }
                }   
            }
            
            double toss = uniform(gen);
            if (toss < deactProb)
            {
                return true;
            }
            else
            {
                return false;
            }   
        }

        Virus* Reproduce(double popDens, ResistVirus* ptrtovrs)
        {
            map<string, bool>::iterator itr;

            double toss = uniform(gen);
            if (toss < repMaxProb * (1 - popDens))
            {
                ResistVirus new_virus = ResistVirus(repMaxProb, deactProb, mutProb, treatments);
                ptrtovrs = &new_virus;
                return ptrtovrs;
                for (itr = new_virus.treatments.begin(); itr != new_virus.treatments.end(); ++itr)
                {
                    toss = uniform(gen);
                    if (toss < mutProb)
                    {
                        itr->second = not itr->second;
                    }
                }
                return ptrtovrs;
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
    
    int maxVir; // problem jest taki że takich wirusów są normalnie tryliony

    vector<string> presentTrts;
    vector<SimpleVirus> virs;

    void update()
    {
        int virn = virs.size();
        double vird = virs.size();
        vird /= maxVir;
        cout<<vird<<endl;

        for (int i = 0; i < virn; i++)
        {
            SimpleVirus* ptrtovrs;
            ptrtovrs = virs[i].Reproduce(vird, ptrtovrs);
            if (ptrtovrs != nullptr)
            {
            SimpleVirus nvir = *ptrtovrs;
            virs.push_back(nvir);
            }
            if(virs[i].doesDeact())
            {
                virs.erase(virs.begin() + i);
                i --;
            }
        }
        
    }

    void Infect(double repr, double deact)
    {
        SimpleVirus vir = SimpleVirus(repr, deact);
        virs.push_back(vir);
    }

    void NewTreatment(string trt)
    {
        presentTrts.push_back(trt);
    }

    void RemoveTreatment(string trt)
    {
        vector<string>::iterator it = find(presentTrts.begin(), presentTrts.end(), trt);

        if(it != presentTrts.end())
        {
            presentTrts.erase(it);
        }
        // else
        // {
                // coś może robić jak już nie ma tego leczenia
        // }
    }
};


int main()
{
    Patient pacjent0;
    pacjent0.NewTreatment("vaccine1");
    pacjent0.NewTreatment("vaccine2");
    pacjent0.NewTreatment("vaccine3");
    pacjent0.RemoveTreatment("vaccine2");
    pacjent0.Infect(0.3,0.02);
    print_strvector(pacjent0.presentTrts);
    cout<<pacjent0.virs.size()<<endl;
    for (int ii = 0; ii < 40; ii++)
    {
        pacjent0.update();
        cout<<pacjent0.virs.size()<<endl;
        cout<<"elo"<<endl;
    }
    
    return 0;
}