#include <iostream>
#include <cmath>

using namespace std;

double* aver (double* arr, int size, double& average)
{
    double a = 0;
    double* adr = &arr[0];
    double dif[size];

    for(int i = 0; i < size; i++)
    {
        a += arr[i];
    }
    a = a/size;
    average = a;
    for(int i = 0; i < size; i++)
    {
        dif[i] = abs(arr[i] - a);
    }
    for(int i = 0; i < size; i++)
    {
        if (dif[i] < a)
        {
            a = dif[i];
        }
    }
        for(int i = 0; i < size; i++)
    {
        if (dif[i] == a)
        {
            adr = &arr[i];
        }
    }
    return adr;
}

int main()
{
    double dable[] = {1,1.1,22.2,31.32,4.5,323,3.44};
    int roz = sizeof(dable) / sizeof(dable[0]);
    double average = 0;
    const double* p = aver (dable, roz, average);
    cout << " AVERAGE: " << average << endl;
    cout << " ADDRESS: " << p << " VALUE: " << *p << endl;
    return 0;
}