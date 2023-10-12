#include "sim_c.h"

using namespace std;


int main()
{    

    Simulation sim;
    sim.resolution_constant = 0.06; // how big is the step in the RK4
    sim.setSpeedDistribution(0,2700); // mean, standard deviation
    sim.run(10000, false); // int number of trials, bool export data for plotting (much slower)
    sim.histogram(); // make histogram

    try
    {
        system("python plotting.py"); // you have to be in the same directory as this file
    }
    catch(...)
    {}
    
    return 0;
}