#include <iostream>
#include <cmath>
#include <random>

using namespace std;

/*
###
### Gra w kółko i krzyżyk przeciwko losowo ruszającemu się komputerowi
### Autor: Wojciech Śmiałek
###
*/

string s_znak(int zn)
{
    if(zn == 1)
    {
        return "kolko";
    }
    else
    {
        return "krzyzyk";
    }
    
}

void print_board(int array[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (array[i][j] == 1)
            {
                cout << " O ";
                if (j<2)
                {
                    cout << '|';
                }
                
            }
            else if (array[i][j] == 2)
            {
                cout << " X ";
                if (j<2)
                {
                    cout << '|';
                }
            }
            else
            {
                cout << "   ";
                if (j<2)
                {
                    cout << '|';
                }
            }
        }
        if (i<2)
        {
            cout << endl << "===========" << endl;
        }
        else
        {
            cout << endl << endl;
        }
        
    }
}

int get_znak()
{
    string inp;
    cout << "Wybierz czym chcesz grac: 1 - kolko, 2 - krzyzyk (domyslnie kolko)" << endl;
    getline(cin, inp);
    for(char& c : inp) 
    {
        if (c == '2')
        {
            return 2;
        }
    }
    return 1;
}

void get_move(int& wie, int& kol, int array[3][3], int zn)
{
    bool wierszok = false;
    bool kolumnok = false;
    string inp;
    cout << "Podaj pole, gdzie chcesz postawic " << s_znak(zn) << ", w formacie (wiersz,kolumna): " << endl;
    getline(cin, inp);
    for(char& c : inp) 
        {
            if (c - '0' < 4 && c - '0' > 0)
            {
                if (wierszok)
                {
                    if(kolumnok)
                    {
                        wie = -1;
                        break;
                    }
                    kol = c - '0' - 1;
                    kolumnok = true;
                }
                else
                {
                    wie = c - '0' - 1;
                    wierszok = true;
                }
            }
            else if ((c - '0' >= 4 && c - '0' <= 9)||(c == '0'))
            {
                wie = -1;
                break;
            }
        }
    if(wie == -1 || kol == -1)
        {
            cout << "blednie podane wspolrzedne!" << endl;
            wie = -1;
            kol = -1;
        }
    else if(array[wie][kol] != 0)
    {
        cout << "pole jest juz zajete!" << endl;
        wie = -1;
        kol = -1;
    }
}

void get_ai_move(int& wie, int& kol, int array[3][3])
{
    while (true)
    {
        int los = 0;
        random_device rd;
        default_random_engine engine(rd());
        uniform_int_distribution<> distr(0,8);
        los = distr(engine);
        if (array[los%3][los/3] != 0)
        {
            continue;
        }
        else
        {
            wie = los%3;
            kol = los/3;
            break;
        }
    }
}

bool doesitend(int array[3][3], int& wyg)
{
    for (int i = 0; i < 3; i++)
    {
        if (array[i][0] != 0 && array[i][0] == array[i][1] && array[i][1] == array[i][2])
        {
            wyg = array[i][0];
            return true;
        }
        else if (array[0][i] != 0 && array[0][i] == array[1][i] && array[1][i] == array[2][i])
        {
            wyg = array[0][i];
            return true;
        }        
    }
    if(array[1][1] != 0)
    if ((array[0][0] == array[1][1] && array[1][1] == array[2][2])||(array[0][2] == array[1][1] && array[1][1] == array[2][0]))
    {
        wyg = array[1][1];
        return true;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {   
            if (array[i][j] == 0)
            {
                return false;
            }
        }
    }
    wyg = 0;
    return true;
}

int main()
{
    int znak = get_znak(); // Gracz wybiera kółko lub krzyżyk
    int wiersz;
    int kolumna;
    int wygrany = 0;

    // Inicjacja planszy wypełnionej zerami, czyli pustymi polami
    int plansza[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            plansza[i][j] = 0;
        }
    }

    // Jeśli gracz wybrał krzyżyk, komputer rusza się pierwszy
    if(znak == 2)
    {
        get_ai_move(wiersz, kolumna, plansza);
        plansza[wiersz][kolumna] = 3 - znak;
        print_board(plansza);
    }
    while (true)
    {
        wiersz = -1;
        kolumna = -1;
        while(wiersz == -1)
        {
            // Wartość -1 oznacza brak poprawnego ruchu. Pytamy gracza aż do uzyskania poprawnego ruchu
            get_move(wiersz, kolumna, plansza, znak);
        }
        plansza[wiersz][kolumna] = znak;
        cout << endl;
        if(doesitend(plansza, wygrany))
        {
            break;
        }
        get_ai_move(wiersz, kolumna, plansza);
        plansza[wiersz][kolumna] = 3 - znak;
        print_board(plansza);
        if(doesitend(plansza, wygrany))
        {
            break;
        }
    }
    print_board(plansza);

    if(wygrany == 0)
    {
        cout << "Remis!";
    }
    else
    {
        cout << "Zwyciezca: " << s_znak(wygrany);
    }
}