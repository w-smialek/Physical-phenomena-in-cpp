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

        SimpleVirus* Reproduce(double popDens, SimpleVirus& stest)
        {
            double toss = uniform(gen);
            if (toss < repMaxProb * (1 - popDens))
            {
                stest = SimpleVirus(repMaxProb, deactProb);
                return &stest;
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
            // cout<<"ini"<<endl;
        }

        bool doesDeact(vector<string> treatment)
        {
            std::map<string, bool>::iterator ite;
            for(int a = 0; a < treatment.size(); a++)
            {
                ite = treatments.find(treatment[a]);
                if (ite->second == false)
                {
                    return true;
                }
                
            }
            
            // map<string, bool>::iterator itr;
            
            // for (int ip = 0; ip < treatment.size(); ip++)
            // {
            //     for (itr = treatments.begin(); itr != treatments.end(); ++itr) 
            //     {
            //         if (itr->first == treatment[ip])
            //         {
            //             // cout<<"pacjent i z listy same"<<endl;
            //             if (not itr->second)
            //             {
            //                 return true;
            //             }
            //         }
            //     }   
            // }
            
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

        ResistVirus* Reproduce(double popDens, ResistVirus& test)
        {
            map<string, bool>::iterator itr;

            double toss = uniform(gen);
            if (toss < repMaxProb * (1 - popDens))
            {
                test = ResistVirus(repMaxProb, deactProb, mutProb, treatments);

                for (itr = test.treatments.begin(); itr != test.treatments.end(); ++itr)
                {
                    toss = uniform(gen);
                    if (toss < mutProb)
                    {
                        bool b = test.treatments[itr->first];
                        test.treatments[itr->first] = not b;
                    }
                }
            return &test;
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
    vector<ResistVirus> rvirs;

    // void update()
    // {
    //     int virn = virs.size();
    //     double vird = virs.size();
    //     vird /= maxVir;
    //     cout<<vird<<endl;

    //     for (int i = 0; i < virn; i++)
    //     {
    //         SimpleVirus stest;
    //         SimpleVirus* ptrtovrs = virs[i].Reproduce(vird, stest);
    //         if (ptrtovrs != nullptr)
    //         {
    //         SimpleVirus nvir = *ptrtovrs;
    //         virs.push_back(nvir);
    //         }
    //         if(virs[i].doesDeact())
    //         {
    //             virs.erase(virs.begin() + i);
    //             i --;
    //             virn--;
    //         }
    //     }
        
    // }

        void update()
    {
        int virn = rvirs.size();
        double vird = rvirs.size();
        vird /= maxVir;
        cout<<"vird "<<vird<<endl;

        for (int i = 0; i < virn; i++)
        {
            ResistVirus test;
            ResistVirus* ptrtovrs = rvirs[i].Reproduce(vird, test);
            if (ptrtovrs != nullptr)
            {
            // ResistVirus nvir = *ptrtovrs;
            rvirs.push_back(*ptrtovrs);
            }
            if(rvirs[i].doesDeact(presentTrts))
            {
                rvirs.erase(rvirs.begin() + i);
                i --;
                virn --;
                if (rvirs.size() == 0)
                {
                    break;
                }
            }
        }
    }


    // void Infect(double repr, double deact)
    // {
    //     SimpleVirus vir = SimpleVirus(repr, deact);
    //     virs.push_back(vir);
    // }
    void Infect(double repr, double deact, double mut, map<string, bool> trts)
    {
        ResistVirus vir = ResistVirus(repr, deact, mut, trts);
        rvirs.push_back(vir);
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
    map<string, bool> existent_treatments;
    existent_treatments.insert(pair<string, bool>("vaccine1", true));
    existent_treatments.insert(pair<string, bool>("vaccine2", true));
    existent_treatments.insert(pair<string, bool>("medyczna marihuana", false));
    existent_treatments.insert(pair<string, bool>("dmt", false));
    existent_treatments.insert(pair<string, bool>("amantadyna", false));

    Patient pacjent0 = Patient(100);
    pacjent0.NewTreatment("vaccine1");
    pacjent0.NewTreatment("vaccine2");
    pacjent0.Infect(0.3,0.01,0.01, existent_treatments);

    cout<<"trts u pacjenta: "<<endl;
    print_strvector(pacjent0.presentTrts);
    cout<<"======"<<endl;

    for (int ii = 0; ii < 300; ii++)
    {
        pacjent0.update();
        cout<<ii<<"  n of virs: "<<pacjent0.virs.size()<<endl;
    }
    
    return 0;
}