#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <windows.h>

using namespace std;

int mw, mk;

/* Struktura Kwadrat: wiersz, kolumna, wierzcholek_przodek, kolumna_przodek, wartosc, koszt_dojscia */
struct Kwadrat
{
    int w, k, w_p, k_p, koszt;
    float wartosc;
};

/* Zadelkarowanie wektorow: Przeszkody, Otwarta, Zamknieta, Trasa */
vector <Kwadrat> Przeszkody;
vector <Kwadrat> Otwarta;
vector <Kwadrat> Zamknieta;
vector <Kwadrat> Trasa;

// Funkcje pomocnicze
// Zwraca czy element jest przeszkoda
bool przeszkoda(int w, int k)
{
    int dlugosc = Przeszkody.size();
    for(int i=0; i<dlugosc; i++)
        if(Przeszkody[i].w == w && Przeszkody[i].k == k)
        return true;
    return false;
}
// Zwraca czy element znajduje sie na liscie otwartej
bool otwarta(int w, int k)
{
    int dlugosc = Otwarta.size();
    for(int i=0; i<dlugosc; i++)
        if(Otwarta[i].w == w && Otwarta[i].k == k)
        return true;
    return false;
}
// Zwraca czy element znajduje sie na liscie zamknietej
bool zamknieta(int w, int k)
{
    int dlugosc = Zamknieta.size();
    for(int i=0; i<dlugosc; i++)
        if(Zamknieta[i].w == w && Zamknieta[i].k == k)
        return true;
    return false;
}
// Liczy heurystyke
float wylicz_wartosc(int w, int k, int koszt)
{
    int a = ((w*w) - (2*w*mw) + (mw*mw)) + ((k*k) - (2*k*mk) + (mk*mk));
    return koszt + pow(a, 1.0/2.0);
}
// Zwraca koszt ruchu przodka (lista zamknieta)
int koszt(int w, int k)
{
    int dlugosc = Zamknieta.size();
    for(int i=0; i<dlugosc; i++)
        if(Zamknieta[i].w == w && Zamknieta[i].k == k)
            return Zamknieta[i].koszt;
}
// Dodanie elementu do listy otwartej
void dodaj(int w, int k, int y, int x)
{
    Kwadrat Otw;
    Otw.w =  w + y;
    Otw.k = k + x;
    Otw.w_p = w;
    Otw.k_p = k;
    Otw.koszt = koszt(w, k) + 1;
    Otw.wartosc = wylicz_wartosc(Otw.w, Otw.k, Otw.koszt);
    Otwarta.push_back(Otw);
}
// Zwraca wartosc elementu z listy otwartej
float wartosc(int w, int k)
{
    int dlugosc = Otwarta.size();
    for(int i=0; i<dlugosc; i++)
        if(Otwarta[i].w == w && Otwarta[i].k == k)
            return Otwarta[i].wartosc;
}
// Usuwa element z listy otwartej
void usun(int w, int k)
{
    int dlugosc = Otwarta.size();
    for(int i; i<dlugosc; i++)
        if(Otwarta[i].w == w && Otwarta[i].k == k)
        {
            Otwarta.erase(Otwarta.begin()+i);
            break;
        }

}
// Podmienia element z listy otwartej jesli nowa wartosc jest mniejsza
void podmien(int w, int k, int x, int y)
{
               /* Wartosc nowa  */             /* Wartosc stara  */
    if(wylicz_wartosc(w+x, k+y, koszt(w, k)+1) < wartosc(w+x, k+y))
    {
        // Usun element z listy otwartej i dodaj nowy
        usun(w+x, k+y);
        dodaj(w, k, x, y);
    }
}
//----------------------------
/* Wczytanie danych z pliku */
void wczytaj_gridTXT()
{
    fstream plik;
    plik.open("grid.txt", ios::in);

    if(!plik.good())
    {
        cout << "Plik grid.txt nie istnieje!";
        exit(0);
    }

    string linia, wiersze[20]; int nr_wiersza=1;
    while(getline(plik, linia))
    {
        wiersze[nr_wiersza-1] = linia;
        int nr_kolumny = 0;
        for(int i=0; i<41; i+=2)
        {
            Kwadrat bufor;
            bufor.w = 20 - nr_wiersza;
            bufor.k = nr_kolumny;

            if(linia[i]-48 == 5)
                Przeszkody.push_back(bufor);

            nr_kolumny++;
        }
        nr_wiersza++;
    }

    //Kolorowanie tekstu w konsoli
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for(int i=0; i<20; i++)
    {
        for(int j=0; j<40; j+=2)
        {
            //if(wiersze[i][j] == '3')
                //SetConsoleTextAttribute(hConsole, 2);
            if(wiersze[i][j] == '0')
                SetConsoleTextAttribute(hConsole, 3);
            else if(wiersze[i][j] == '5')
                SetConsoleTextAttribute(hConsole, 4);
            //else if(wiersze[i][j] == 'S' || wiersze[i][j] == 'C')
                //SetConsoleTextAttribute(hConsole, 6);
            cout << wiersze[i][j] << ' ';
                SetConsoleTextAttribute(hConsole, 7);
        }
        cout << endl;
    }

    plik.close();
}

/* Pobierz start */
/* Pobierz mete */
/* Sprawdz start i mete */
void sprawdz_start_i_mete(int sw, int sk)
{
    if(sw == mw && sk == mk)
    {
        cout << "Start i meta nie moga wystepowac w tym samym miejscu!";
        exit(0);
    }

    if(sw < 0 || sw > 19)
    {
        cout << "Wiersz startu zostal podany niepoprawnie!";
        exit(0);
    }

    if(mw < 0 || mw > 19)
    {
        cout << "Wiersz mety zostal podany niepoprawnie!";
        exit(0);
    }

    if(sk < 0 || sk > 19)
    {
        cout << "Kolumna startu zostal podany niepoprawnie!";
        exit(0);
    }

    if(mk < 0 || mk > 19)
    {
        cout << "Kolumna mety zostal podany niepoprawnie!";
        exit(0);
    }

    if(przeszkoda(sw, sk))
    {
        cout << "Start nie moze wystepowac w miejscu przeszkody!";
        exit(0);
    }

    if(przeszkoda(mw, mk))
    {
        cout << "Meta nie moze wystepowac w miejscu przeszkody!";
        exit(0);
    }
}

/* Ustaw start */
void ustaw_start(int w, int k)
{
    Kwadrat Start;
    Start.w =  w;
    Start.k = k;
    //Start.w_p = -1;
    //Start.k_p = -1;
    Start.koszt = 0;
    Start.wartosc = wylicz_wartosc(Start.w, Start.k, Start.koszt);
    Zamknieta.push_back(Start);
}

/* Dodanie pol do listy otwartej */
void dodaj_do_listy_otwartej(int sw, int sk)
{
    // Gora
    if(sw+1 <= 19 && !zamknieta(sw+1, sk) && !przeszkoda(sw+1, sk))
    {
        if(otwarta(sw+1, sk))
            //Sprawdz czy nowa wartosc jest mniejsza, jesli tak to ja podmien
            podmien(sw, sk, 1, 0);

        else
            dodaj(sw, sk, 1, 0);
    }
    // Dol
    if(sw-1 >= 0 && !zamknieta(sw-1, sk) && !przeszkoda(sw-1, sk))
    {
        if(otwarta(sw-1, sk)) podmien(sw, sk, -1, 0);
        else dodaj(sw, sk, -1, 0);
    }
    // Lewo
    if(sk-1 >= 0 && !zamknieta(sw, sk-1) && !przeszkoda(sw, sk-1))
    {
        if(otwarta(sw, sk-1)) podmien(sw, sk, 0, -1);
        else dodaj(sw, sk, 0, -1);
    }
    // Prawa
    if(sk+1 <= 19 && !zamknieta(sw, sk+1) && !przeszkoda(sw, sk+1))
    {
        if(otwarta(sw, sk+1)) podmien(sw, sk, 0, 1);
        else dodaj(sw, sk, 0, 1);
    }
}
/* Zwracanie indeksu elementu z listy otwartej o najmniejszej wartosci */
int najmniejszy_element_listy_otwartej()
{
    int dlugosc = Otwarta.size(), _min = Otwarta[dlugosc-1].wartosc, index = dlugosc-1;
    if(dlugosc == 0) return -1;

    for(int i=dlugosc-1; i>=0; i--)
        if(Otwarta[i].wartosc < _min)
        {
            _min = Otwarta[i].wartosc;
            index = i;
        }
    return index;
}
/* Utworzenie nowego punktu startowego */
void dodaj_do_listy_zamknietej_i_usun_z_otwartej(int index)
{
    Zamknieta.push_back(Otwarta[index]);

    Otwarta.erase(Otwarta.begin() + index);
}
/* Odtwarzanie trasy wyznaczonej przez algorytm A* */
void odtworz_trase(int sw, int sk)
{
    // Wypchniece mete do tablicy
    Trasa.push_back(Zamknieta[Zamknieta.size()-1]);

    do{
        // Na podstawie w_p i k_p odnajdz porzednika
        int dlugosc = Zamknieta.size();
        for(int i=0; i<dlugosc; i++)
            if(Zamknieta[i].w == Trasa[Trasa.size()-1].w_p && Zamknieta[i].k == Trasa[Trasa.size()-1].k_p)
            {
                // Wypchnij poprzednika do Trasa
                Trasa.push_back(Zamknieta[i]);
                break;
            }
    /* Jesli ostatni element w i k w Trasa jest rowny ze start to przerwij*/
    }while( Trasa[Trasa.size()-1].w != sw || Trasa[Trasa.size()-1].k != sk );
}
//wczytaj_trase_do_pliku();
void odczytaj_trase()
{
    fstream plik;
    plik.open("grid.txt", ios::in);

    string linia; int nr_wiersza=1;
    string wiersze[20];
    while(getline(plik, linia))
    {
        wiersze[nr_wiersza-1] = linia;
        nr_wiersza++;
    }

    plik.close();

    int dlugosc = Trasa.size();
    for(int i=0; i<dlugosc; i++)
        wiersze[19 - Trasa[i].w][Trasa[i].k * 2] = '3';

    wiersze[19 - Trasa[Trasa.size()-1].w][Trasa[Trasa.size()-1].k * 2] = 'S';
    wiersze[19 - Trasa[0].w][Trasa[0].k * 2] = 'C';

    //Kolorowanie tekstu w konsoli
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for(int i=0; i<20; i++)
    {
        for(int j=0; j<40; j+=2)
        {
            if(wiersze[i][j] == '3')
                SetConsoleTextAttribute(hConsole, 2);
            else if(wiersze[i][j] == '0')
                SetConsoleTextAttribute(hConsole, 3);
            else if(wiersze[i][j] == '5')
                SetConsoleTextAttribute(hConsole, 4);
            else if(wiersze[i][j] == 'S' || wiersze[i][j] == 'C')
                SetConsoleTextAttribute(hConsole, 6);
            cout << wiersze[i][j] << ' ';
                SetConsoleTextAttribute(hConsole, 7);
        }
        cout << endl;
    }
}

int main()
{
    int sw, sk;
    wczytaj_gridTXT();

    cout << "Podaj punkt startowy(wiersz, kolumna): ";
    cin >> sw >> sk;
    cout << "Podaj cel(wiersz, kolumna): ";
    cin >> mw >> mk;
    sprawdz_start_i_mete(sw, sk);

    ustaw_start(sw, sk);

    int ssw = sw, ssk = sk;
    do{
        dodaj_do_listy_otwartej(sw, sk);
        int index = najmniejszy_element_listy_otwartej();
        if(index == -1) break;
        sw = Otwarta[index].w;
        sk = Otwarta[index].k;
        dodaj_do_listy_zamknietej_i_usun_z_otwartej(index);
    }while((sw != mw || sk != mk));

    if(najmniejszy_element_listy_otwartej() == -1)
        cout << "Cel nieosiagalny!";
    else
    {
        odtworz_trase(ssw, ssk);
        system("cls");
        odczytaj_trase();
        cout << "Start:(" << ssw << ',' << ssk << ')' << endl;
        cout << "Cel:  (" << mw << ',' << mk << ')';
    }

    return 0;
}
