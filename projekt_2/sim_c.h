#include <valarray>
#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <random>

using namespace std;

/*

Global constants

*/

//distance
double H = 1.49598e11;
double d = 3.84400000e8;

//mass
double M_s = 1.989e30;
double M_z = 5.9722e24 ;
double M_k = 7.3477e22 ;

//time
double monat = 2.36059e6;
double om_z = 2*M_PI/(12*monat);
double om_k = 2*M_PI/monat;

//gravitational constant
double G = 6.67408e-11;

//differential equation dimensionality
const size_t NDIM = 4;

/*

 Definitions, Classes, Functions

*/

typedef std::valarray<double> Valarr;

class Simulation {

    private:
    
        double angle1;
        double angle2;
        double speed; 
        double phi;
        double t;
        double r_s;
        double r_z;
        double r_k;
        double v;
        double dt;
        double px;
        double py;
        int stepNumber;
        int nSteps = 700;
        bool hit = false;

        mt19937 gen = mt19937((random_device())());
        normal_distribution<double> normal = normal_distribution<double>(0, 2700);
        uniform_real_distribution<double> uniform = uniform_real_distribution<double>(0,2*M_PI);

        Valarr RHS( const double t, const Valarr X, const double phi)
        {
            Valarr output(NDIM);
            output[0] = X[2];
            output[1] = X[3];
            output[2] = -((G*M_z*(-H + X[0]))/pow(pow(H - X[0],2) + pow(X[1],2),1.5) - 
                (G*M_k*(d*cos(om_k*t + phi) + H - X[0]))/pow(pow(d*cos(om_k*t + phi) + H - X[0],2) + pow(-X[1] + d*sin(om_k*t + phi),2),1.5) + 
                (G*M_s*X[0])/pow(pow(X[0],2) + pow(X[1],2),1.5));
            output[3] = -G*(X[1]*M_z/pow(pow(H - X[0],2) + pow(X[1],2),1.5) + 
                M_k*(-X[1] + d*sin(om_k*t + phi))/pow(pow(d*cos(om_k*t + phi) + H - X[0],2) + pow(-X[1] + d*sin(om_k*t + phi),2),1.5) + 
                X[1]*M_s/pow(pow(X[0],2) + pow(X[1],2),1.5));
            return output;
        }

        void printVector(vector<int> v)
        {
            for (int i: v)
                std::cout << i << ' ';
            cout<<endl;
        }

        void diceroll()
        {
            // get random values
            angle1 = uniform(gen);
            angle2 = uniform(gen);
            speed = normal(gen);
            phi = uniform(gen);
        }


    public:

        double resolution_constant = 0.06;

        vector<double> angles;

        void run(int hits, bool eksport)
        {
            Valarr X;

            for (int j = 0; j < hits; j++)
            {
                hit = false;

                while(not hit)
                {
                    diceroll();
                    double init[] = {H + 3*d*cos(angle1), 3*d*sin(angle1),speed*cos(angle2),speed*sin(angle2)};
                    Valarr X( init, NDIM );

                    /* sets random initial states and simulates particle trajectories 
                    until a hit to a lunar surface is detected */

                    t = 0.;
                    stepNumber = 1;
                    if(eksport)
                    {
                    ofstream trajectory("tr.txt");
                    while (stepNumber<nSteps) // thanks to the limit of steps, simulations of the orbiting particles will be terminated
                    {
                        r_s = pow(pow(X[0],2) + pow(X[1],2),0.5);
                        r_z = pow(pow(H - X[0],2) + pow(X[1],2),0.5);
                        r_k = pow(pow(d*cos(om_k*t + phi) + H - X[0],2) + pow(-X[1] + d*sin(om_k*t + phi),2),0.5);
                        v = pow(pow(X[2],2) + pow(X[3],2) ,0.5);
                        
                        dt = resolution_constant*min({r_s,r_z,r_k})/(v+1000); // step depends on the distance to the closest body and speed

                        Valarr k1 = RHS( t, X, phi);
                        Valarr k2 = RHS( t + dt / 2.0,  X + dt / 2.0 * k1, phi );
                        Valarr k3 = RHS( t + dt / 2.0, X + dt / 2.0 * k2, phi );
                        Valarr k4 = RHS( t + dt, X + dt * k3, phi );

                        /* RHS is the vector function of the ODE.
                        the Runge–Kutta fourth-order method is used */
                    
                        X += dt / 6.0 * ( k1 + 2.0 * k2 + 2.0 * k3 + k4 );
                        t += dt;

                        trajectory << std::setprecision(10);
                        trajectory << X[0]/d << "," << X[1]/d << endl; /* in this case, the data is exported. 
                        It makes sense for a single simulation, because only last one is saved and it slows down the process a lot */
                        ++stepNumber;

                        if(r_z > 3*d)
                        {
                            break;
                        }
                        if(r_k < 3.5e6)
                        {
                            hit = true;
                            break;
                        }
                        if(r_z < 12.7e6)
                        {
                            break;
                        }
                        /* the simulation ends when the particle will move away
                        or it hits the earth or it hits the moon, in which case two loops break*/
                    }
                    trajectory << phi/(2*M_PI) << "," << t/monat << endl;
                    trajectory.close();
                    }
                    else
                    {
                    while (stepNumber<nSteps)
                    {
                        r_s = pow(pow(X[0],2) + pow(X[1],2),0.5);
                        r_z = pow(pow(H - X[0],2) + pow(X[1],2),0.5);
                        r_k = pow(pow(d*cos(om_k*t + phi) + H - X[0],2) + pow(-X[1] + d*sin(om_k*t + phi),2),0.5);
                        v = pow(pow(X[2],2) + pow(X[3],2) ,0.5);
                        
                        dt = resolution_constant*min({r_s,r_z,r_k})/(v+1000);

                        Valarr k1 = RHS( t, X, phi);
                        Valarr k2 = RHS( t + dt / 2.0,  X + dt / 2.0 * k1, phi );
                        Valarr k3 = RHS( t + dt / 2.0, X + dt / 2.0 * k2, phi );
                        Valarr k4 = RHS( t + dt, X + dt * k3, phi );
                    
                        X += dt / 6.0 * ( k1 + 2.0 * k2 + 2.0 * k3 + k4 );
                        t += dt;

                        ++stepNumber;

                        if(r_z > 3*d)
                        {
                            break;
                        }
                        if(r_k < 3.5e6)
                        {
                            hit = true;
                            break;
                        }
                        if(r_z < 12.7e6)
                        {
                            break;
                        }
                    }
                    }
                    px = X[0];
                    py = X[1];
                }
                /* the angle between final position of particle and the line between earth and the final position of moon
                is saved, as well as that position */
                double angle = atan2(py - d*sin(om_k*t + phi), px - H - d*cos(om_k*t + phi));
                angle += phi - (t/monat)*2*M_PI;
                angle = fmod(angle, 2*M_PI);
                if(angle < 0)
                {angle += 2*M_PI;}
                cout<<"angle: "<<angle*360/(2*M_PI)<<endl;
                angles.push_back(angle*360/(2*M_PI));
                cout<<"number of hits: "<<j+1<<endl;
            }
        }

        void histogram()
        {
            vector<int> histo(360,0);

            for (int ii = 0; ii < angles.size(); ii++)
            {
                histo[round(angles[ii])] ++;
            }

            cout<<"Histogram, kolejne przedzialy po 1 stopien od 0 do 360: "<<endl;
            printVector(histo);
            
            ofstream histogram("hs.txt");
            for (int ii = 0; ii < 360; ii++)
            {
                histogram << histo[ii] << ",";
            }
            histogram.close();
        }

        void setSpeedDistribution(double mean, double stddev)
        {
            normal = normal_distribution<double>(mean, stddev);
        }

};