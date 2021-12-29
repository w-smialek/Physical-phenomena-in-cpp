#include <iostream>

using namespace std;

void pascal(int table[][27])
{
    for(int i = 1; i<13; i++)
    {
        for(int j = 0; j<27; j++)
        {
            table[i][j] = (table[i-1][j-1] + table[i-1][j+1]);
        }
    }
}

int main()
{
    int trojkat [13] [27];
    for(int i = 0; i<27; i++)
    {   
        if(i == 13)
        {
            trojkat[0][i] = 1;
        }
        else
        {
            trojkat[0][i] = 0;
        }
    }
    
    pascal(trojkat);
    
    for(int i = 0; i<13; i++)
    {
        for(int j = 0; j<27; j++)
        {
            if (trojkat[i][j] != 0)
            {
                cout << trojkat[i][j] << " ";
            }
        }
        cout << endl;
    }
    return 0;
}
