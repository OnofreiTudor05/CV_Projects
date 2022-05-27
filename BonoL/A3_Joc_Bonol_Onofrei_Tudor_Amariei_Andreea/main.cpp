#include <iostream>
#include <fstream>
#include <winbgim.h>
#include <ctime>
#include <MMSystem.h>
#include <cstring>

#define LUNGIME_MAXIMA_TABLA 5
#define LUNGIME_TABLA 4
#define NUMAR_MAXIM_MUTARI 100005
#define NUMAR_MAXIM_BUTOANE_ATINSE 20
#define LUNGIME_MAXIMA_TEXT_BUTON 100
#define NUMAR_MAXIM_BUTOANE_FOLOSITE 50
#define NUMAR_BUTOANE_PIESA 4
#define NUMAR_MAXIM_JUCATORI 4

using namespace std;

ifstream citesteDinFisier;

struct mutari
{
    int Tabla[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA];
};

struct nume{
    char nume[300];
};

struct RGB
{
    int r;
    int g;
    int b;
};

struct sistem_cartezian
{
    int x;
    int y;
};

struct dimensiune
{
    int x;
    int y;
    int lungime;
    int latime;
};

struct buton
{
    RGB background_color;
    RGB outline_color;
    RGB text_color;
    dimensiune pozitie;
    int outline_border_width;
    char nume_interior[LUNGIME_MAXIMA_TEXT_BUTON];
};

struct cerc
{
    RGB background_color;
    RGB outline_color;
    int raza;
    int outline_border_width;
};

struct mutareAI{
    int indice_piesa;
    int linie;
    int coloana;
};

struct setari{
    bool sonor;
    sistem_cartezian rezolutie;
    int indice_limba;
    int indice_rezolutie;
};

struct tabla_de_joc
{
    buton TablaDeDesenat[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA];
    buton ChenarTabla;
    buton ChenarJucator[NUMAR_MAXIM_JUCATORI];
    int TablaNumerica[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA];
    int TuraJucator;
    RGB culoare_disc;
    RGB culoare_outline_disc;
    int modJoc;
    char numeJucator1[LUNGIME_MAXIMA_TEXT_BUTON];
    char numeJucator2[LUNGIME_MAXIMA_TEXT_BUTON];
    setari SetariJoc;
};


bool eInMeniu, eInInstructiuni, eInJoc, eInSetari, eInConfiguratii;
int indice_vechi = 1, indice_rezolutie =1;
nume FisiereButoaneLimba[7][6];

void convertesteIntSiSalveazaInString(int numar, char sir[100]){
    char sir1[100]="";
    int v[100], copie_numar, n, st, dr, i;
    n = 0;
    copie_numar = numar;
    do{
        n++;
        v[n] = copie_numar%10;
        copie_numar /= 10;
    }while(copie_numar != 0);
    st = 1; dr = n;
    while(st < dr){
        swap(v[st], v[dr]);
        st++;
        dr--;
    }
    for(i = 0; i<n; i++){
        sir1[i] = v[i+1] + '0';
    }
    strcpy(sir, sir1);
}

void citesteButonDinFisierulCurentDeschis(buton &A){
    citesteDinFisier >> A.background_color.r >> A.background_color.g >> A.background_color.b;
    citesteDinFisier >> A.outline_color.r >> A.outline_color.g >> A.outline_color.b;
    citesteDinFisier >> A.text_color.r >> A.text_color.g >> A.text_color.b;
    citesteDinFisier >> A.outline_border_width;
    citesteDinFisier >> A.pozitie.x >> A.pozitie.y >> A.pozitie.lungime >> A.pozitie.latime;
    citesteDinFisier.get();
    citesteDinFisier.getline(A.nume_interior, LUNGIME_MAXIMA_TEXT_BUTON-1);
    if(!strcmp(A.nume_interior, "#"))
    {
        strcpy(A.nume_interior, "");
    }
    else
    {
        int latime_text, lungime_text;
        lungime_text = textwidth(A.nume_interior);
        if(lungime_text > A.pozitie.lungime)
        {
            A.pozitie.x -= A.outline_border_width;
            A.pozitie.lungime = lungime_text + 3*A.outline_border_width;
        }
        latime_text = textheight(A.nume_interior);
        if(latime_text > A.pozitie.latime)
        {
            A.pozitie.y -= A.outline_border_width;
            A.pozitie.latime = latime_text + 3*A.outline_border_width;
        }
    }
}

void citeste_variabile(int M[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA], int &n, buton P[NUMAR_MAXIM_BUTOANE_FOLOSITE])
{
    citesteDinFisier.open("matrice.in");
    int i, j;
    citesteDinFisier >> n;
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            citesteDinFisier >> M[i][j];
        }
    }
    citesteDinFisier.close();
    citesteDinFisier.open("butoane.in");
    for(i = 1; i<=n; i++)
    {
        citesteButonDinFisierulCurentDeschis(P[i]);
    }
    citesteDinFisier.close();
}

void sterge_fereastra(int color)
{
    setbkcolor(color);
    cleardevice();
}

void deseneazaButon2(buton &A)
{
    int poly[10];
    int latime_text, lungime_text;
    lungime_text = textwidth(A.nume_interior);
    if(lungime_text > A.pozitie.lungime)
    {
        A.pozitie.x -= A.outline_border_width;
        A.pozitie.lungime = lungime_text + 3*A.outline_border_width;
    }
    latime_text = textheight(A.nume_interior);
    if(latime_text > A.pozitie.latime)
    {
        A.pozitie.y -= A.outline_border_width;
        A.pozitie.latime = latime_text + 3*A.outline_border_width;
    }
    poly[0] = A.pozitie.x;
    poly[1] = A.pozitie.y;
    poly[2] = A.pozitie.x+A.pozitie.lungime;
    poly[3] = A.pozitie.y;
    poly[4] = A.pozitie.x+A.pozitie.lungime;
    poly[5] = A.pozitie.y+A.pozitie.latime;
    poly[6] = A.pozitie.x;
    poly[7] = A.pozitie.y+A.pozitie.latime;
    setcolor(COLOR(A.outline_color.r, A.outline_color.g, A.outline_color.b));
    setfillstyle(SOLID_FILL, COLOR(A.outline_color.r, A.outline_color.g, A.outline_color.b));
    fillpoly(4, poly);

    poly[0] = A.pozitie.x+A.outline_border_width;
    poly[1] = A.pozitie.y+A.outline_border_width;

    poly[2] = A.pozitie.x+A.pozitie.lungime-A.outline_border_width;
    poly[3] = A.pozitie.y+A.outline_border_width;

    poly[4] = A.pozitie.x+A.pozitie.lungime-A.outline_border_width;
    poly[5] = A.pozitie.y+A.pozitie.latime-A.outline_border_width;

    poly[6] = A.pozitie.x+A.outline_border_width;
    poly[7] = A.pozitie.y+A.pozitie.latime-A.outline_border_width;
    setcolor(COLOR(A.background_color.r, A.background_color.g, A.background_color.b));
    setfillstyle(SOLID_FILL, COLOR(A.background_color.r, A.background_color.g, A.background_color.b));
    fillpoly(4, poly);
    setbkcolor(COLOR(A.background_color.r, A.background_color.g, A.background_color.b));
    setcolor(COLOR(A.text_color.r, A.text_color.g, A.text_color.b));
    outtextxy(A.pozitie.x + (A.pozitie.lungime-textwidth(A.nume_interior))/2, A.pozitie.y + (A.pozitie.latime - textheight(A.nume_interior))/2, A.nume_interior);
}

void deseneaza_disc(dimensiune pozitie_disc, int raza1, int raza2, int outline_border, RGB culoare_fundal, RGB culoare_outline)
{
    setfillstyle(SOLID_FILL, COLOR(culoare_outline.r, culoare_outline.g, culoare_outline.b));
    setcolor(COLOR(culoare_outline.r, culoare_outline.g, culoare_outline.b));
    fillellipse(pozitie_disc.x + raza1, pozitie_disc.y + raza2, raza1, raza2);
    setcolor(COLOR(culoare_fundal.r, culoare_fundal.g, culoare_fundal.b));
    setfillstyle(SOLID_FILL, COLOR(culoare_fundal.r, culoare_fundal.g, culoare_fundal.b));
    fillellipse(pozitie_disc.x + raza1, pozitie_disc.y + raza2, raza1-outline_border, raza2-outline_border);
}

void copiaza_matrice(int Sursa[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA], int Destinatie[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA])
{
    int i, j;
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            Destinatie[i][j] = Sursa[i][j];
        }
    }
}

int mutari_posibile_jucatori(int tura, sistem_cartezian Piese[8][8], int Tabla[5][5])
{
    int mutari, i, j, ok_piesa, k, ii, poz_x, poz_y, Mutari[5][5];
    mutari = 0;
    for(i = 1; i<=4; i++)
    {
        for(j = 1; j<=4; j++)
        {
            if(Tabla[i][j] != tura)
            {
                Mutari[i][j] = Tabla[i][j];
            }
            else
            {
                Mutari[i][j] = 0;
            }
        }
    }
    for(k = 1; k<=8; k++)
    {
        for(i = 1; i<=4; i++)
        {
            for(j = 1; j<=4; j++)
            {
                ok_piesa = 1;
                for(ii = 1; ii<=4 && ok_piesa; ii++)
                {
                    poz_x = i + Piese[k][ii].x;
                    poz_y = j + Piese[k][ii].y;
                    if(poz_x < 1 || poz_x > 4 || poz_y < 1 || poz_y > 4 || Mutari[poz_x][poz_y] != 0)
                    {
                        ok_piesa = 0;
                    }
                }
                if(ok_piesa)
                {
                    mutari++;
                }
            }
        }
    }
    mutari = mutari - 1;
    return mutari;
}

void afiseaza_matrice(int n, int M[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA])
{
    int i, j;
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            cout << M[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}

bool suntTableEgale(int A[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA], int B[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA])
{
    int i, j;
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            if(A[i][j] != B[i][j])
            {
                return 0;
            }
        }
    }
    return 1;
}

void returneazaCeaMaiBunaMutare(int tura, sistem_cartezian Piese[8][8], int Tabla[5][5], int CeaMaiBunaMutare[5][5]){
    int contor_discuri, minim, mutari, i, j, ok_piesa, k, ii, jj, poz_x, poz_y, Mutari[5][5], copieMutari[5][5], copieMutari1[5][5], ceaMaiBunaConfiguratie[5][5], ceaMaiBunaConfiguratieDisc1[5][5], ceaMaiBunaConfiguratieDisc2[5][5];
    bool gasit_minim;
    sistem_cartezian PozitiiDiscuri[5];
    mutari = 0;
    minim = 999999;
    contor_discuri = 0;
    for(i = 1; i<=4; i++)
    {
        for(j = 1; j<=4; j++)
        {
            if(Tabla[i][j] == 1)
            {
                contor_discuri++;
                PozitiiDiscuri[contor_discuri].x = i;
                PozitiiDiscuri[contor_discuri].y = j;
            }
            if(Tabla[i][j] != tura)
            {
                Mutari[i][j] = Tabla[i][j];
            }
            else
            {
                Mutari[i][j] = 0;
            }
        }
    }
    for(k = 1; k<=8; k++)
    {
        for(i = 1; i<=4; i++)
        {
            for(j = 1; j<=4; j++)
            {
                copiaza_matrice(Mutari, copieMutari);
                ok_piesa = 1;
                for(ii = 1; ii<=4 && ok_piesa; ii++)
                {
                    poz_x = i + Piese[k][ii].x;
                    poz_y = j + Piese[k][ii].y;
                    if(poz_x < 1 || poz_x > 4 || poz_y < 1 || poz_y > 4 || Mutari[poz_x][poz_y] != 0)
                    {
                        ok_piesa = 0;
                    }
                }
                if(ok_piesa)
                {
                    for(ii = 1; ii<=4; ii++)
                    {
                        poz_x = i + Piese[k][ii].x;
                        poz_y = j + Piese[k][ii].y;
                        copieMutari[poz_x][poz_y] = tura;
                    }
                    if(mutari_posibile_jucatori(5-tura, Piese, copieMutari) <= minim && !suntTableEgale(Tabla, copieMutari))
                    {
                        minim = mutari_posibile_jucatori(5-tura, Piese, copieMutari);
                        copiaza_matrice(copieMutari, ceaMaiBunaConfiguratie);
                    }
                }
            }
        }
    }
    gasit_minim = 0;
    for(i = 1; i<=2; i++){
        copiaza_matrice(ceaMaiBunaConfiguratie, copieMutari1);
        copieMutari1[PozitiiDiscuri[i].x][PozitiiDiscuri[i].y] = 0;
        for(ii = 1; ii<=4; ii++){
            for(jj = 1; jj<=4; jj++){
                if(copieMutari1[ii][jj] == 0){
                    copieMutari1[ii][jj] = 1;
                    if(mutari_posibile_jucatori(5-tura, Piese, copieMutari1) <= minim){
                        minim = mutari_posibile_jucatori(5-tura, Piese, copieMutari1);
                        if(!gasit_minim){
                            gasit_minim = 1;
                            copiaza_matrice(copieMutari1, ceaMaiBunaConfiguratieDisc1);
                            copiaza_matrice(copieMutari1, ceaMaiBunaConfiguratieDisc2);
                        }
                        else{
                            if(i == 1){
                                copiaza_matrice(copieMutari1, ceaMaiBunaConfiguratieDisc1);
                            }
                            else if(i == 2){
                                copiaza_matrice(copieMutari1, ceaMaiBunaConfiguratieDisc2);
                            }
                        }
                    }
                    else{
                        copiaza_matrice(ceaMaiBunaConfiguratieDisc1, ceaMaiBunaConfiguratieDisc2);
                    }
                    copieMutari1[ii][jj] = 0;
                }
            }
        }
    }
    if(mutari_posibile_jucatori(5-tura, Piese, ceaMaiBunaConfiguratieDisc1) < mutari_posibile_jucatori(5-tura, Piese, ceaMaiBunaConfiguratieDisc2))
    {
        copiaza_matrice(ceaMaiBunaConfiguratieDisc1, ceaMaiBunaConfiguratie);
    }
    else
    {
        copiaza_matrice(ceaMaiBunaConfiguratieDisc2, ceaMaiBunaConfiguratie);
    }
    copiaza_matrice(ceaMaiBunaConfiguratie, CeaMaiBunaMutare);
}

bool eCursorulPeButon(int x, int y, buton A)
{
    if(x > A.pozitie.x-A.outline_border_width
            && x < A.pozitie.x + A.pozitie.lungime + A.outline_border_width
            && y > A.pozitie.y - A.outline_border_width
            && y < A.pozitie.y + A.pozitie.latime + A.outline_border_width)
    {
        return 1;
    }
    return 0;
}

void adaugaMutare(int &indice, mutari vectorMutari[NUMAR_MAXIM_MUTARI], int Mutare[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA])
{
    int i, j;
    indice++;
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            vectorMutari[indice].Tabla[i][j] = Mutare[i][j];
        }
    }
}

void deseneazaTabla(int M[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA], buton Tabla[LUNGIME_MAXIMA_TABLA][LUNGIME_MAXIMA_TABLA], buton butonTabla, buton butonPiesa1, buton butonPiesa2, RGB culoare_fundal_disc, RGB culoare_outline_disc)
{
    int i, j;
    int xx, yy;
    xx = butonTabla.pozitie.x;
    yy = butonTabla.pozitie.y;
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            if(M[i][j] == 0)
            {
                Tabla[i][j] = butonTabla;
            }
            else if(M[i][j] == 1)
            {
                Tabla[i][j] = butonTabla;
            }
            else if(M[i][j] == 2)
            {
                Tabla[i][j] = butonPiesa1;
            }
            else if(M[i][j] == 3)
            {
                Tabla[i][j] = butonPiesa2;
            }
            Tabla[i][j].pozitie.x = xx + (j-1)*(butonTabla.pozitie.lungime +  butonTabla.outline_border_width/5 + 1);
            Tabla[i][j].pozitie.y = yy + (i-1)*(butonTabla.pozitie.latime  +  butonTabla.outline_border_width/5 + 1);
        }
    }
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            deseneazaButon2(Tabla[i][j]);
            if(M[i][j] == 1)
            {
                deseneaza_disc(Tabla[i][j].pozitie, Tabla[i][j].pozitie.lungime/2, Tabla[i][j].pozitie.latime/2, Tabla[i][j].outline_border_width, culoare_fundal_disc, culoare_outline_disc);
            }
        }
    }
    return;
}

bool evalueaza_piese_pe_linie(sistem_cartezian A[20])
{
    int i, j;
    bool ok1, ok2;
    for(i = 1; i<4; i++)
    {
        for(j = i+1; j<=4; j++)
        {
            if(A[i].x > A[j].x)
            {
                swap(A[i], A[j]);
            }
            else if(A[i].x == A[j].x)
            {
                if(A[i].y > A[j].y)
                {
                    swap(A[i], A[j]);
                }
            }
        }
    }
    if(A[1].x == A[2].x && A[2].x == A[3].x && A[2].y == A[1].y+1 && A[3].y == A[2].y+1)
    {
        if(A[4].x == A[1].x + 1 || A[4].x == A[1].x-1 || A[4].x == A[3].x + 1 || A[4].x == A[3].x-1)
        {
            if(A[4].y == A[1].y || A[4].y == A[3].y)
            {
                return 1;
            }
        }
    }
    else if(A[2].x == A[3].x && A[3].x == A[4].x && A[3].y == A[2].y+1 && A[4].y == A[3].y+1)
    {
        if(A[1].x == A[2].x + 1 || A[1].x == A[2].x-1 || A[1].x == A[4].x + 1 || A[4].x == A[4].x-1)
        {
            if(A[1].y == A[2].y || A[1].y == A[4].y)
            {
                return 1;
            }
        }
    }
    return 0;
}

void sterge_tura(int M[5][5], int tura)
{
    int i, j;
    for(i = 1; i<=4; i++)
    {
        for(j = 1; j<=4; j++)
        {
            if(M[i][j] == tura)
            {
                M[i][j] = 0;
            }
        }
    }
}

void seteazaCuloare(RGB &A, int culoareDetectata)
{
    A.r = RED_VALUE(culoareDetectata);
    A.g = GREEN_VALUE(culoareDetectata);
    A.b = BLUE_VALUE(culoareDetectata);
}

void scrieInTextBox(buton &A){
    bool ok, modificat;
    char tasta, text[50]="";
    int n;
    ok = 1;
    tasta = 'A';
    n = 0;
    strcpy(A.nume_interior, text);
    seteazaCuloare(A.background_color, COLOR(255, 0, 0));
    deseneazaButon2(A);
    while(ok){
        if(kbhit()){
            tasta = getch();
            if((int)tasta == 27){
                ok = 0;
            }
            else if((int)tasta == 8){
                modificat = 1;
                if(n != 0){
                   text[n] = ' ';
                   strcpy(text+n, "");
                   n--;
                   strcpy(text+n, "");
                }
                else if(n == 0){
                    n = 0;
                    strcpy(text, "");
                }
            }
            else if((int)tasta >= 32 && (int)tasta <= 126 && n <= 15){
                modificat = 1;
                text[n] = tasta;
                n++;
            }
            else if((int)tasta == 13){
                ok = 0;
                seteazaCuloare(A.background_color, COLOR(100, 100, 100));
                strcpy(A.nume_interior, text);
                deseneazaButon2(A);
            }
            else{
                n = n;
            }
        }
            if(modificat){
                strcpy(A.nume_interior, text);
                modificat = 0;
                deseneazaButon2(A);
            }
            if(n == 0){
                strcpy(text, " ");
                strcpy(A.nume_interior, text);
            }
        }
}

bool evalueaza_piese_pe_coloana(sistem_cartezian A[20])
{
    int i, j;
    for(i = 1; i<4; i++)
    {
        for(j = i+1; j<=4; j++)
        {
            if(A[i].y > A[j].y)
            {
                swap(A[i], A[j]);
            }
            else if(A[i].y == A[j].y)
            {
                if(A[i].x > A[j].x)
                    swap(A[i], A[j]);
            }
        }
    }

    if(A[1].y == A[2].y && A[2].y == A[3].y && A[2].x == A[1].x+1 && A[3].x == A[2].x+1)
    {
        if(A[4].y == A[1].y + 1 || A[4].y == A[1].y - 1 || A[4].y == A[3].y + 1 || A[4].y == A[3].y-1)
        {
            if(A[4].x == A[1].x || A[4].x == A[3].x)
            {
                return 1;
            }
        }
    }
    else if(A[2].y == A[3].y && A[3].y == A[4].y && A[3].x == A[2].x+1 && A[4].x == A[3].x+1)
    {
        if(A[1].y == A[2].y + 1 || A[1].y == A[2].y - 1 || A[1].y == A[4].y + 1 || A[4].y == A[4].y-1)
        {
            if(A[1].x == A[2].x || A[1].x == A[4].x)
            {
                return 1;
            }
        }
    }
    return 0;
}

bool suntInAceeasiPozitie(sistem_cartezian A[5], sistem_cartezian B[5])
{
    int i;
    bool egal;
    egal = 1;
    for(i = 1; i<=4 && !egal; i++)
    {
        if(A[i].x != B[i].x || A[i].y != B[i].y)
        {
            egal = 0;
        }
    }
    return egal;
}

void sterge_chenar_de_pe_tabla(tabla_de_joc &TablaJocCurent, int x, int y)
{
    buton CopieButonTabla;
    CopieButonTabla = TablaJocCurent.ChenarTabla;
    CopieButonTabla.pozitie = TablaJocCurent.TablaDeDesenat[x][y].pozitie;
    TablaJocCurent.TablaNumerica[x][y] = 0;
    deseneazaButon2(CopieButonTabla);
}

void sterge_mutarea_anterioara(tabla_de_joc &TablaJocCurent)
{
    for(int iv = 1; iv<=LUNGIME_TABLA; iv++)
    {
        for(int jv = 1; jv<=LUNGIME_TABLA; jv++)
        {
            if(TablaJocCurent.TablaNumerica[iv][jv] == TablaJocCurent.TuraJucator)
            {
                sterge_chenar_de_pe_tabla(TablaJocCurent, iv, jv);
            }
        }
    }
}

int returneazaCuloareCaInt(RGB culoare)
{
    return COLOR(culoare.r, culoare.g, culoare.b);
}

void mutaPiesaJucator(tabla_de_joc &TablaJocCurent, RGB culoare_fundal_disc, RGB culoare_disc, mutari V[10005], int &indice_mutare)
{
    delay(10);
    bool apasat, atinse[20][20], gata;
    int ii, jj, i, j, iii, jjj, iv, jv, xx, yy, apasate, Mutare[5][5];
    buton Hasura, CopieButonTabla, CopieButonJucator[4];
    sistem_cartezian A[100], A1[100], Piese_pe_tabla[5][5];
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            atinse[i][j] = 0;
        }
    }
    apasate = 0;
    gata = 0;
    CopieButonJucator[3] = TablaJocCurent.ChenarJucator[3];
    CopieButonTabla = TablaJocCurent.ChenarTabla;
    CopieButonJucator[2] = TablaJocCurent.ChenarJucator[2];
    while(!gata)
    {
        delay(10);
        for(i = 1; i<=LUNGIME_TABLA; i++)
        {
            for(j = 1; j<=LUNGIME_TABLA; j++)
            {
                if(ismouseclick(WM_MOUSEMOVE))
                {
                    getmouseclick(WM_MOUSEMOVE, xx, yy);
                }
                else if(ismouseclick(WM_LBUTTONDOWN))
                {
                    getmouseclick(WM_LBUTTONDOWN, xx, yy);
                    apasat = 1;
                }
                else if(ismouseclick(WM_LBUTTONUP))
                {
                    getmouseclick(WM_LBUTTONUP, xx, yy);
                    apasat = 0;
                    for(ii = 1; ii<=LUNGIME_TABLA; ii++)
                    {
                        for(jj = 1; jj<=LUNGIME_TABLA; jj++)
                        {
                            atinse[ii][jj] = 0;
                        }
                    }
                }
                if(eCursorulPeButon(xx, yy, TablaJocCurent.TablaDeDesenat[i][j]) && apasat && (TablaJocCurent.TablaNumerica[i][j] == 0 || TablaJocCurent.TablaNumerica[i][j] == TablaJocCurent.TuraJucator))
                {
                    atinse[i][j] = 1;
                    apasate = 1;
                    A[apasate].x = A1[apasate].x = i;
                    A[apasate].y = A1[apasate].y = j;
                    Hasura = TablaJocCurent.TablaDeDesenat[i][j];
                    seteazaCuloare(Hasura.background_color, COLOR(200, 200, 200));
                    seteazaCuloare(Hasura.outline_color, COLOR(100, 100, 100));
                    deseneazaButon2(Hasura);

                    while(apasat)
                    {
                        if(ismouseclick(WM_MOUSEMOVE))
                        {
                            getmouseclick(WM_MOUSEMOVE, xx, yy);
                        }
                        else if(ismouseclick(WM_LBUTTONUP))
                        {
                            getmouseclick(WM_LBUTTONUP, xx, yy);
                            delay(10);
                            apasat = 0;
                            apasate = 0;
                        }
                        for(ii = 1; ii<=LUNGIME_TABLA; ii++)
                        {
                            for(jj = 1; jj<=LUNGIME_TABLA; jj++)
                            {
                                if(atinse[ii][jj] == 0 && eCursorulPeButon(xx, yy, TablaJocCurent.TablaDeDesenat[ii][jj]) && (TablaJocCurent.TablaNumerica[ii][jj] == 0 || TablaJocCurent.TablaNumerica[ii][jj] == TablaJocCurent.TuraJucator))
                                {
                                    atinse[ii][jj] = 1;
                                    apasate++;
                                    A[apasate].x = A1[apasate].x = ii;
                                    A[apasate].y = A1[apasate].y = jj;
                                    Hasura = TablaJocCurent.TablaDeDesenat[ii][jj];
                                    seteazaCuloare(Hasura.background_color, COLOR(200, 200, 200));
                                    seteazaCuloare(Hasura.outline_color, COLOR(100, 100, 100));
                                    deseneazaButon2(Hasura);
                                }
                                if(apasate == NUMAR_BUTOANE_PIESA && ismouseclick(WM_LBUTTONUP))
                                {
                                    getmouseclick(WM_LBUTTONUP, xx, yy);
                                    delay(10);
                                    if(evalueaza_piese_pe_linie(A))
                                    {
                                        copiaza_matrice(TablaJocCurent.TablaNumerica, Mutare);
                                        sterge_tura(Mutare, TablaJocCurent.TuraJucator);
                                        for(int ib=1; ib<=NUMAR_BUTOANE_PIESA; ib++)
                                        {
                                            Mutare[A[ib].x][A[ib].y] = TablaJocCurent.TuraJucator;
                                        }
                                        if(!suntTableEgale(Mutare, TablaJocCurent.TablaNumerica))
                                        {
                                            for(int iv = 1; iv<=LUNGIME_TABLA; iv++)
                                            {
                                                for(int jv = 1; jv<=LUNGIME_TABLA; jv++)
                                                {
                                                    if(TablaJocCurent.TablaNumerica[iv][jv] == TablaJocCurent.TuraJucator)
                                                    {
                                                        TablaJocCurent.TablaNumerica[iv][jv] = 0;
                                                        CopieButonTabla.pozitie = TablaJocCurent.TablaDeDesenat[iv][jv].pozitie;
                                                        deseneazaButon2(CopieButonTabla);
                                                    }
                                                }
                                            }
                                            for(int iv = 1; iv<=NUMAR_BUTOANE_PIESA; iv++)
                                            {
                                                TablaJocCurent.TablaNumerica[A[iv].x][A[iv].y] = TablaJocCurent.TuraJucator;
                                                CopieButonJucator[TablaJocCurent.TuraJucator].pozitie = TablaJocCurent.TablaDeDesenat[A[iv].x][A[iv].y].pozitie;
                                                deseneazaButon2(CopieButonJucator[TablaJocCurent.TuraJucator]);
                                            }
                                            apasate = 0;
                                            apasat = 0;
                                            gata = 1;
                                            adaugaMutare(indice_mutare, V, TablaJocCurent.TablaNumerica);
                                        }
                                        else
                                        {
                                            deseneazaTabla(TablaJocCurent.TablaNumerica, TablaJocCurent.TablaDeDesenat, TablaJocCurent.ChenarTabla, TablaJocCurent.ChenarJucator[2], TablaJocCurent.ChenarJucator[3], TablaJocCurent.culoare_disc, TablaJocCurent.culoare_outline_disc);
                                            apasat = false;
                                            apasate = 0;
                                            delay(10);
                                        }
                                    }
                                    else if(evalueaza_piese_pe_coloana(A1))
                                    {
                                        copiaza_matrice(TablaJocCurent.TablaNumerica, Mutare);
                                        sterge_tura(Mutare, TablaJocCurent.TuraJucator);
                                        for(int ib=1; ib<=NUMAR_BUTOANE_PIESA; ib++)
                                        {
                                            Mutare[A1[ib].x][A1[ib].y] = TablaJocCurent.TuraJucator;
                                        }
                                        if(!suntTableEgale(Mutare, TablaJocCurent.TablaNumerica))
                                        {
                                            sterge_mutarea_anterioara(TablaJocCurent);
                                            for(int iv = 1; iv<=NUMAR_BUTOANE_PIESA; iv++)
                                            {
                                                TablaJocCurent.TablaNumerica[A1[iv].x][A1[iv].y] = TablaJocCurent.TuraJucator;
                                                CopieButonJucator[TablaJocCurent.TuraJucator].pozitie = TablaJocCurent.TablaDeDesenat[A1[iv].x][A1[iv].y].pozitie;
                                                deseneazaButon2(CopieButonJucator[TablaJocCurent.TuraJucator]);
                                            }
                                            apasate = 0;
                                            apasat = 0;
                                            gata = 1;
                                            adaugaMutare(indice_mutare, V, TablaJocCurent.TablaNumerica);
                                        }
                                        else
                                        {
                                            deseneazaTabla(TablaJocCurent.TablaNumerica, TablaJocCurent.TablaDeDesenat, TablaJocCurent.ChenarTabla, TablaJocCurent.ChenarJucator[2], TablaJocCurent.ChenarJucator[3], TablaJocCurent.culoare_disc, TablaJocCurent.culoare_outline_disc);
                                            delay(10);
                                            apasate = 0;
                                            apasat = 0;
                                        }
                                    }
                                    else
                                    {
                                        deseneazaTabla(TablaJocCurent.TablaNumerica, TablaJocCurent.TablaDeDesenat, TablaJocCurent.ChenarTabla, TablaJocCurent.ChenarJucator[2], TablaJocCurent.ChenarJucator[3], TablaJocCurent.culoare_disc, TablaJocCurent.culoare_outline_disc);
                                        delay(10);
                                        apasate = 0;
                                    }
                                }
                                else if(apasate != NUMAR_BUTOANE_PIESA && ismouseclick(WM_LBUTTONUP))
                                {
                                    getmouseclick(WM_LBUTTONUP, xx, yy);
                                    deseneazaTabla(TablaJocCurent.TablaNumerica, TablaJocCurent.TablaDeDesenat, TablaJocCurent.ChenarTabla, TablaJocCurent.ChenarJucator[2], TablaJocCurent.ChenarJucator[3], TablaJocCurent.culoare_disc, TablaJocCurent.culoare_outline_disc);
                                    delay(10);
                                    apasate = apasat = 0;
                                }
                            }
                        }
                    }
                }
                else if(!apasat)
                {
                    for(iii = 1; iii<=LUNGIME_TABLA; iii++)
                    {
                        for(jjj = 1; jjj<=LUNGIME_TABLA; jjj++)
                        {
                            atinse[iii][jjj] = 0;
                        }
                    }
                }
            }
        }
    }

    int gata_pus, luat_disc;
    luat_disc = gata_pus = 0;


    while(!gata_pus)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, xx, yy);
            for(ii = 1; ii<=LUNGIME_TABLA; ii++)
            {
                for(jj = 1; jj<=LUNGIME_TABLA; jj++)
                {
                    if(eCursorulPeButon(xx, yy, TablaJocCurent.TablaDeDesenat[ii][jj]) && TablaJocCurent.TablaNumerica[ii][jj] == 1 && !apasat)
                    {
                        apasat = 1;
                        TablaJocCurent.TablaNumerica[ii][jj] = 0;
                        CopieButonTabla.pozitie = TablaJocCurent.TablaDeDesenat[ii][jj].pozitie;
                        deseneazaButon2(CopieButonTabla);
                    }
                    else if(eCursorulPeButon(xx, yy, TablaJocCurent.TablaDeDesenat[ii][jj]) && TablaJocCurent.TablaNumerica[ii][jj] == 0 && apasat)
                    {
                        apasat = 0;
                        TablaJocCurent.TablaNumerica[ii][jj] = 1;
                        gata_pus = 1;
                        deseneaza_disc(TablaJocCurent.TablaDeDesenat[ii][jj].pozitie, TablaJocCurent.TablaDeDesenat[ii][jj].pozitie.lungime/2, TablaJocCurent.TablaDeDesenat[ii][jj].pozitie.latime/2, TablaJocCurent.TablaDeDesenat[ii][jj].outline_border_width, culoare_disc, culoare_fundal_disc);

                    }
                }
            }
        }
    }

    TablaJocCurent.TuraJucator = 5-TablaJocCurent.TuraJucator;
    delay(10);
}

void mutaPiesaAI(tabla_de_joc &TablaJocCurent, sistem_cartezian Piese[8][8]){
    int i, j, MatriceNumerica[5][5];

    returneazaCeaMaiBunaMutare(TablaJocCurent.TuraJucator, Piese, TablaJocCurent.TablaNumerica, MatriceNumerica);
    TablaJocCurent.TuraJucator = 5-TablaJocCurent.TuraJucator;
    copiaza_matrice(MatriceNumerica, TablaJocCurent.TablaNumerica);
    deseneazaTabla(TablaJocCurent.TablaNumerica, TablaJocCurent.TablaDeDesenat, TablaJocCurent.ChenarTabla, TablaJocCurent.ChenarJucator[2], TablaJocCurent.ChenarJucator[3], TablaJocCurent.culoare_disc, TablaJocCurent.culoare_outline_disc);
}

void centreaza_buton_pe_latime(buton &A)
{
    A.pozitie.x = (800 - A.pozitie.lungime)/2;
}

void reseteazaButoaneApasate(bool apasate[10], int pozitie)
{
    int i;
    apasate[pozitie] = true;
    for(i = 1; i<=8; i++)
    {
        if(i != pozitie)
        {
            apasate[i] = false;
        }
    }
}

void scaleazaSiRepozitioneazaButon(buton &A, sistem_cartezian RezolutieNoua, sistem_cartezian RezolutieVeche){
    if((RezolutieNoua.x != RezolutieVeche.x) || (RezolutieVeche.y != RezolutieNoua.y)){
        A.pozitie.x = (int)((A.pozitie.x * RezolutieNoua.x*1.0/RezolutieVeche.x));
        A.pozitie.y = (int)((A.pozitie.y * RezolutieNoua.y*1.0/RezolutieVeche.y));
        A.pozitie.lungime = (int)((A.pozitie.lungime * RezolutieNoua.x*1.0/RezolutieVeche.x));
        A.pozitie.latime = (int)((A.pozitie.latime * RezolutieNoua.y*1.0/RezolutieVeche.y));
        A.outline_border_width = 5;
    }
}

void configureaza_mod_joc(tabla_de_joc &TablaJocCurent)
{
    buton MesajInapoi, ChenarPlayerVsPlayer, ChenarPlayerVsAI, Tabla[5][5], PatratTabla, PatratJucator1, PatratJucator2, AlegereCuloriJoc[10], ChenarCuloriRGB, CasetaNumeJucator2, MesajNumeJucator2, CasetaNumeJucator1, MesajNumeJucator1, MesajCuloriDisc, CuloareDisc, CuloareOutlineDisc, MesajCuloarePiesa2, CuloarePiesa2, CuloareOutlinePiesa2, CuloareTabla, CuloareOutlineTabla, CuloarePiesa1, CuloareOutlinePiesa1, MesajStartJoc, MesajCuloriTabla, MesajCulori1, JucatorVsJucator, JucatorVsComputer, Mesaj;
    int xx, yy, modJocAles;
    int M[5][5];
    RGB culoareDisc, culoareOutlineDisc, culoareRosu, culoareNegru;
    sistem_cartezian RezolutieCurenta, RezolutieVeche;
    culoareRosu.r = 255;
    culoareNegru.r = culoareNegru.b = culoareNegru.g = culoareRosu.g = culoareRosu.b = 0;

    RezolutieCurenta = TablaJocCurent.SetariJoc.rezolutie;
    RezolutieVeche.x = 800; RezolutieVeche.y = 600;
    readimagefile("FundalMeniuPlay.jpg", 0, 0, RezolutieCurenta.x, RezolutieCurenta.y);

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

    citesteDinFisier.open(FisiereButoaneLimba[TablaJocCurent.SetariJoc.indice_limba][3].nume, ios::in);

    citesteButonDinFisierulCurentDeschis(JucatorVsJucator);
    citesteButonDinFisierulCurentDeschis(JucatorVsComputer);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    citesteButonDinFisierulCurentDeschis(Mesaj);

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    citesteButonDinFisierulCurentDeschis(MesajCulori1);
    citesteButonDinFisierulCurentDeschis(MesajCuloriTabla);
    citesteButonDinFisierulCurentDeschis(MesajStartJoc);
    citesteButonDinFisierulCurentDeschis(CuloarePiesa1);
    citesteButonDinFisierulCurentDeschis(CuloareOutlinePiesa1);

    citesteButonDinFisierulCurentDeschis(MesajCuloarePiesa2);
    citesteButonDinFisierulCurentDeschis(MesajCuloriDisc);
    citesteButonDinFisierulCurentDeschis(MesajNumeJucator1);
    citesteButonDinFisierulCurentDeschis(CasetaNumeJucator1);
    citesteButonDinFisierulCurentDeschis(MesajNumeJucator2);
    citesteButonDinFisierulCurentDeschis(CasetaNumeJucator2);
    citesteButonDinFisierulCurentDeschis(PatratTabla);
    citesteButonDinFisierulCurentDeschis(MesajInapoi);

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    centreaza_buton_pe_latime(JucatorVsJucator);
    centreaza_buton_pe_latime(JucatorVsComputer);
    centreaza_buton_pe_latime(Mesaj);


    seteazaCuloare(ChenarPlayerVsPlayer.background_color, COLOR(255, 0, 0));
    seteazaCuloare(ChenarPlayerVsPlayer.outline_color, COLOR(0, 0, 0));

    seteazaCuloare(ChenarPlayerVsAI.background_color, COLOR(255, 255, 255));
    seteazaCuloare(ChenarPlayerVsAI.outline_color, COLOR(0, 0, 0));

    ChenarPlayerVsPlayer.pozitie.latime = ChenarPlayerVsPlayer.pozitie.lungime = JucatorVsJucator.pozitie.latime;
    ChenarPlayerVsPlayer.outline_border_width = JucatorVsJucator.outline_border_width;
    ChenarPlayerVsPlayer.pozitie.x = JucatorVsJucator.pozitie.x-ChenarPlayerVsPlayer.pozitie.lungime-ChenarPlayerVsPlayer.outline_border_width;
    ChenarPlayerVsPlayer.pozitie.y = JucatorVsJucator.pozitie.y;

    ChenarPlayerVsAI.pozitie.latime = ChenarPlayerVsAI.pozitie.lungime = JucatorVsComputer.pozitie.latime;
    ChenarPlayerVsAI.outline_border_width = JucatorVsComputer.outline_border_width;
    ChenarPlayerVsAI.pozitie.x = JucatorVsComputer.pozitie.x-ChenarPlayerVsAI.pozitie.lungime-ChenarPlayerVsAI.outline_border_width;
    ChenarPlayerVsAI.pozitie.y = JucatorVsComputer.pozitie.y;
    strcpy(ChenarPlayerVsAI.nume_interior, "");

    ChenarCuloriRGB.pozitie.x = 0;
    ChenarCuloriRGB.pozitie.y = Mesaj.pozitie.y+Mesaj.pozitie.latime+2;
    ChenarCuloriRGB.pozitie.lungime = ChenarCuloriRGB.pozitie.latime = 200;


    scaleazaSiRepozitioneazaButon(ChenarPlayerVsAI, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(ChenarPlayerVsPlayer, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(Mesaj, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(JucatorVsComputer, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(JucatorVsJucator, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(ChenarCuloriRGB, RezolutieCurenta, RezolutieVeche);

    readimagefile("Culori.jpg", 0, Mesaj.pozitie.y+Mesaj.pozitie.latime+2, (int)((200 * RezolutieCurenta.x*1.0/RezolutieVeche.x)), (int)((200 * RezolutieCurenta.y*1.0/RezolutieVeche.y)));

    deseneazaButon2(ChenarPlayerVsAI);
    deseneazaButon2(ChenarPlayerVsPlayer);
    deseneazaButon2(JucatorVsJucator);
    deseneazaButon2(JucatorVsComputer);
    deseneazaButon2(Mesaj);

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 2);
    CuloareOutlineTabla = CuloarePiesa1;
    CuloareTabla = CuloarePiesa1;
    CuloareOutlinePiesa2 = CuloareOutlinePiesa1;
    CuloarePiesa2 = CuloarePiesa1;
    CuloareOutlineDisc = CuloareOutlinePiesa1;
    CuloareDisc = CuloarePiesa1;

    CuloarePiesa1.pozitie.x = MesajCulori1.pozitie.x + MesajCulori1.pozitie.lungime;
    CuloarePiesa1.pozitie.y = MesajCulori1.pozitie.y;

    CuloareOutlinePiesa1.pozitie.x = CuloarePiesa1.pozitie.x + CuloarePiesa1.pozitie.lungime;
    CuloareOutlinePiesa1.pozitie.y = CuloarePiesa1.pozitie.y;

    CuloareTabla.pozitie.x = MesajCuloriTabla.pozitie.x + MesajCuloriTabla.pozitie.lungime;
    CuloareTabla.pozitie.y = MesajCuloriTabla.pozitie.y;

    CuloareOutlineTabla.pozitie.x = CuloareTabla.pozitie.x + CuloareTabla.pozitie.lungime;
    CuloareOutlineTabla.pozitie.y = CuloareTabla.pozitie.y;

    CuloarePiesa2.pozitie.x = MesajCuloarePiesa2.pozitie.x + MesajCuloarePiesa2.pozitie.lungime;
    CuloarePiesa2.pozitie.y = MesajCuloarePiesa2.pozitie.y;

    CuloareOutlinePiesa2.pozitie.x = CuloarePiesa2.pozitie.x + CuloarePiesa2.pozitie.lungime;
    CuloareOutlinePiesa2.pozitie.y = CuloarePiesa2.pozitie.y;

    CuloareDisc.pozitie.x = MesajCuloriDisc.pozitie.x + MesajCuloriDisc.pozitie.lungime;
    CuloareDisc.pozitie.y = MesajCuloriDisc.pozitie.y;

    CuloareOutlineDisc.pozitie.x = CuloareDisc.pozitie.x + CuloareDisc.pozitie.lungime;
    CuloareOutlineDisc.pozitie.y = CuloareDisc.pozitie.y;

    MesajStartJoc.pozitie.x = 600;

    scaleazaSiRepozitioneazaButon(MesajCulori1, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(MesajCuloriTabla, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(MesajStartJoc, RezolutieCurenta, RezolutieVeche);

    deseneazaButon2(MesajCulori1);
    deseneazaButon2(MesajCuloriTabla);
    deseneazaButon2(MesajStartJoc);

    MesajInapoi.pozitie.x = 120;
    MesajInapoi.pozitie.y = 520;

    scaleazaSiRepozitioneazaButon(MesajInapoi, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(MesajInapoi);

    scaleazaSiRepozitioneazaButon(MesajCuloarePiesa2, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(MesajCuloarePiesa2);

    scaleazaSiRepozitioneazaButon(MesajCuloriDisc, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(MesajCuloriDisc);

    scaleazaSiRepozitioneazaButon(MesajNumeJucator1, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(MesajNumeJucator2, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(CasetaNumeJucator1, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(CasetaNumeJucator2, RezolutieCurenta, RezolutieVeche);

    deseneazaButon2(MesajNumeJucator1);
    deseneazaButon2(CasetaNumeJucator1);
    deseneazaButon2(MesajNumeJucator2);
    deseneazaButon2(CasetaNumeJucator2);
    bool ok, apasat[10];
    int culoareDetectata, i, j;
    ok = true;

    for(i = 1; i<=8; i++)
    {
        apasat[i] = false;
    }
    AlegereCuloriJoc[1] = CuloarePiesa1;
    AlegereCuloriJoc[2] = CuloareOutlinePiesa1;
    AlegereCuloriJoc[3] = CuloarePiesa2;
    AlegereCuloriJoc[4] = CuloareOutlinePiesa2;
    AlegereCuloriJoc[5] = CuloareTabla;
    AlegereCuloriJoc[6] = CuloareOutlineTabla;
    AlegereCuloriJoc[7] = CuloareDisc;
    AlegereCuloriJoc[8] = CuloareOutlineDisc;

    PatratTabla.pozitie.y = ChenarCuloriRGB.pozitie.y;

    seteazaCuloare(AlegereCuloriJoc[1].background_color, COLOR(255, 0, 0));
    seteazaCuloare(AlegereCuloriJoc[2].background_color, COLOR(0, 255, 0));
    seteazaCuloare(AlegereCuloriJoc[3].background_color, COLOR(0, 0, 255));
    seteazaCuloare(AlegereCuloriJoc[4].background_color, COLOR(255, 255, 0));
    seteazaCuloare(AlegereCuloriJoc[5].background_color, COLOR(255, 0, 255));
    seteazaCuloare(AlegereCuloriJoc[6].background_color, COLOR(0, 255, 255));
    seteazaCuloare(AlegereCuloriJoc[7].background_color, COLOR(0, 0, 0));
    seteazaCuloare(AlegereCuloriJoc[8].background_color, COLOR(255, 255, 255));

    for(i = 1; i<=8; i++)
    {
        scaleazaSiRepozitioneazaButon(AlegereCuloriJoc[i], RezolutieCurenta, RezolutieVeche);
        deseneazaButon2(AlegereCuloriJoc[i]);
    }

    PatratJucator1 = PatratJucator2 = PatratTabla;
    culoareDisc.r = culoareDisc.g = culoareDisc.b = 200;
    culoareOutlineDisc.r = culoareOutlineDisc.g = culoareOutlineDisc.b = 30;
    for(i = 1; i<=5; i++)
    {
        for(j = 1; j<=5; j++)
        {
            M[i][j] = 0;
        }
    }
    M[1][1] = M[1][2] = M[2][1] = M[3][1] = 2;
    M[1][4] = M[2][4] = 1;
    M[4][1] = M[4][2] = M[4][3] = M[3][3] = 3;
    seteazaCuloare(PatratTabla.background_color, returneazaCuloareCaInt(AlegereCuloriJoc[5].background_color));
    seteazaCuloare(PatratTabla.outline_color, returneazaCuloareCaInt(AlegereCuloriJoc[6].background_color));
    seteazaCuloare(PatratJucator1.background_color, returneazaCuloareCaInt(AlegereCuloriJoc[1].background_color));
    seteazaCuloare(PatratJucator1.outline_color, returneazaCuloareCaInt(AlegereCuloriJoc[2].background_color));
    seteazaCuloare(PatratJucator2.background_color, returneazaCuloareCaInt(AlegereCuloriJoc[3].background_color));
    seteazaCuloare(PatratJucator2.outline_color, returneazaCuloareCaInt(AlegereCuloriJoc[4].background_color));
    seteazaCuloare(culoareDisc, returneazaCuloareCaInt(AlegereCuloriJoc[7].background_color));
    seteazaCuloare(culoareOutlineDisc, returneazaCuloareCaInt(AlegereCuloriJoc[8].background_color));

    scaleazaSiRepozitioneazaButon(PatratTabla, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(PatratJucator1, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(PatratJucator2, RezolutieCurenta, RezolutieVeche);


    deseneazaTabla(M, Tabla, PatratTabla, PatratJucator1, PatratJucator2, culoareDisc, culoareOutlineDisc);
    modJocAles = 2;
    while(ok)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, xx, yy);
            if(eCursorulPeButon(xx, yy, MesajInapoi)){
                ok = 0;
                eInMeniu = 1;
            }
            if(eCursorulPeButon(xx, yy, MesajStartJoc))
            {
                ok = false;
                eInJoc = 1;
            }
            else if(eCursorulPeButon(xx, yy, ChenarPlayerVsAI)){
                modJocAles = 1;
                seteazaCuloare(ChenarPlayerVsAI.background_color, COLOR(255, 0, 0));
                seteazaCuloare(ChenarPlayerVsPlayer.background_color, COLOR(255, 255, 255));
                deseneazaButon2(ChenarPlayerVsAI);
                deseneazaButon2(ChenarPlayerVsPlayer);
            }
            else if(eCursorulPeButon(xx, yy, ChenarPlayerVsPlayer)){
                modJocAles = 2;
                seteazaCuloare(ChenarPlayerVsPlayer.background_color, COLOR(255, 0, 0));
                seteazaCuloare(ChenarPlayerVsAI.background_color, COLOR(255, 255, 255));
                deseneazaButon2(ChenarPlayerVsPlayer);
                deseneazaButon2(ChenarPlayerVsAI);
            }
            else if(eCursorulPeButon(xx, yy, CasetaNumeJucator1)){
                scrieInTextBox(CasetaNumeJucator1);
            }
            else if(eCursorulPeButon(xx, yy, CasetaNumeJucator2)){
                scrieInTextBox(CasetaNumeJucator2);
            }
            for(i = 1; i<=8; i++)
            {
                if(eCursorulPeButon(xx, yy, AlegereCuloriJoc[i]) && !apasat[i])
                {
                    reseteazaButoaneApasate(apasat, i);
                    seteazaCuloare(AlegereCuloriJoc[i].outline_color, COLOR(255, 0, 0));
                    deseneazaButon2(AlegereCuloriJoc[i]);
                }
                else if(eCursorulPeButon(xx, yy, ChenarCuloriRGB) && apasat[i])
                {
                    apasat[i] = false;
                    culoareDetectata = getpixel(xx, yy);
                    seteazaCuloare(AlegereCuloriJoc[i].background_color, culoareDetectata);
                    deseneazaButon2(AlegereCuloriJoc[i]);
                    seteazaCuloare(AlegereCuloriJoc[i].outline_color, BLACK);
                    deseneazaButon2(AlegereCuloriJoc[i]);
                    if(i == 1)
                    {
                        seteazaCuloare(PatratJucator1.background_color, culoareDetectata);
                    }
                    else if(i == 2)
                    {
                        seteazaCuloare(PatratJucator1.outline_color, culoareDetectata);
                    }
                    else if(i == 3)
                    {
                        seteazaCuloare(PatratJucator2.background_color, culoareDetectata);
                    }
                    else if(i == 4)
                    {
                        seteazaCuloare(PatratJucator2.outline_color, culoareDetectata);
                    }
                    else if(i == 5)
                    {
                        seteazaCuloare(PatratTabla.background_color, culoareDetectata);
                    }
                    else if(i == 6)
                    {
                        seteazaCuloare(PatratTabla.outline_color, culoareDetectata);
                    }
                    else if(i == 7)
                    {
                        seteazaCuloare(culoareDisc, culoareDetectata);
                    }
                    else if(i == 8)
                    {
                        seteazaCuloare(culoareOutlineDisc, culoareDetectata);
                    }
                    deseneazaTabla(M, Tabla, PatratTabla, PatratJucator1, PatratJucator2, culoareDisc, culoareOutlineDisc);
                }
            }

        }
    }
    TablaJocCurent.ChenarTabla.background_color = PatratTabla.background_color;
    TablaJocCurent.ChenarTabla.outline_color = PatratTabla.outline_color;
    TablaJocCurent.ChenarJucator[2].background_color = PatratJucator1.background_color;
    TablaJocCurent.ChenarJucator[2].outline_color = PatratJucator1.outline_color;
    TablaJocCurent.ChenarJucator[3].background_color = PatratJucator2.background_color;
    TablaJocCurent.ChenarJucator[3].outline_color = PatratJucator2.outline_color;
    TablaJocCurent.culoare_disc = culoareDisc;
    TablaJocCurent.culoare_outline_disc = culoareOutlineDisc;
    TablaJocCurent.modJoc = modJocAles;
    strcpy(TablaJocCurent.numeJucator1, CasetaNumeJucator1.nume_interior);
    strcpy(TablaJocCurent.numeJucator2, CasetaNumeJucator2.nume_interior);
    if(!strcmp(TablaJocCurent.numeJucator1, "")){
        strcpy(TablaJocCurent.numeJucator1, "111111");
    }
    if(!strcmp(TablaJocCurent.numeJucator2, "")){
        strcpy(TablaJocCurent.numeJucator2, "222222");
    }

    eInConfiguratii = 0;
    citesteDinFisier.close();
    return;
}

void acceseazaInstructiuni(tabla_de_joc TablaJocCurent){
    bool ok;
    buton butonInainte, butonInapoi;
    ok = 1;
    int indicePoza, xx, yy, indiceLimba;
    indicePoza = 1;
    indiceLimba = TablaJocCurent.SetariJoc.indice_limba;
    sistem_cartezian RezolutieVeche, RezolutieCurenta;
    nume NumeImagini[7][4];
    strcpy(NumeImagini[1][1].nume, "Media/Romana/1.jpg");
    strcpy(NumeImagini[1][2].nume, "Media/Romana/2.jpg");
    strcpy(NumeImagini[1][3].nume, "Media/Romana/3.jpg");

    strcpy(NumeImagini[2][1].nume, "Media/Engleza/1.jpg");
    strcpy(NumeImagini[2][2].nume, "Media/Engleza/2.jpg");
    strcpy(NumeImagini[2][3].nume, "Media/Engleza/3.jpg");

    strcpy(NumeImagini[3][1].nume, "Media/Franceza/1.jpg");
    strcpy(NumeImagini[3][2].nume, "Media/Franceza/2.jpg");
    strcpy(NumeImagini[3][3].nume, "Media/Franceza/3.jpg");

    strcpy(NumeImagini[4][1].nume, "Media/Spaniola/1.jpg");
    strcpy(NumeImagini[4][2].nume, "Media/Spaniola/2.jpg");
    strcpy(NumeImagini[4][3].nume, "Media/Spaniola/3.jpg");

    strcpy(NumeImagini[5][1].nume, "Media/Germana/1.jpg");
    strcpy(NumeImagini[5][2].nume, "Media/Germana/2.jpg");
    strcpy(NumeImagini[5][3].nume, "Media/Germana/3.jpg");

    strcpy(NumeImagini[6][1].nume, "Media/Italiana/1.jpg");
    strcpy(NumeImagini[6][2].nume, "Media/Italiana/2.jpg");
    strcpy(NumeImagini[6][3].nume, "Media/Italiana/3.jpg");

    RezolutieVeche.x = 800; RezolutieVeche.y = 600;
    RezolutieCurenta = TablaJocCurent.SetariJoc.rezolutie;

    citesteDinFisier.open(FisiereButoaneLimba[TablaJocCurent.SetariJoc.indice_limba][2].nume, ios::in);

    readimagefile(NumeImagini[indiceLimba][1].nume, 0, 0, RezolutieCurenta.x, RezolutieCurenta.y);
    citesteButonDinFisierulCurentDeschis(butonInapoi);
    butonInapoi.pozitie.x = 0;
    citesteButonDinFisierulCurentDeschis(butonInainte);
    butonInainte.pozitie.x = 800-butonInainte.pozitie.lungime;
    scaleazaSiRepozitioneazaButon(butonInapoi, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(butonInapoi);
    scaleazaSiRepozitioneazaButon(butonInainte, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(butonInainte);
    while(ok){
        if(ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN, xx, yy);
            if(eCursorulPeButon(xx, yy, butonInainte) && indicePoza + 1 <= 3){
                indicePoza++;
                readimagefile(NumeImagini[indiceLimba][indicePoza].nume, 0, 0, RezolutieCurenta.x, RezolutieCurenta.y);
                deseneazaButon2(butonInapoi);
                deseneazaButon2(butonInainte);
            }
            else if(eCursorulPeButon(xx, yy, butonInapoi)){
                if(indicePoza - 1 >= 1){
                    indicePoza--;
                    readimagefile(NumeImagini[indiceLimba][indicePoza].nume, 0, 0, RezolutieCurenta.x, RezolutieCurenta.y);
                    deseneazaButon2(butonInapoi);
                    deseneazaButon2(butonInainte);
                }
                else{
                    ok = 0;
                    eInMeniu = 1;
                }
            }
        }
    }
    eInInstructiuni = 0;
    citesteDinFisier.close();
}

void meniu(tabla_de_joc &TablaJocCurent)
{
    int xx, yy, indice_rezolutie_vechi, lungime_fereastra = 800;
    buton Play, Instructions, Exit, Settings;
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    sistem_cartezian ValoriRezolutii[5];

    ValoriRezolutii[1].x = 800; ValoriRezolutii[1].y = 600;
    ValoriRezolutii[2].x = 1200; ValoriRezolutii[2].y = 700;
    ValoriRezolutii[3].x = 1360; ValoriRezolutii[3].y = 760;
    ValoriRezolutii[4].x = GetSystemMetrics(SM_CXSCREEN) + 5;
    ValoriRezolutii[4].y = GetSystemMetrics(SM_CYSCREEN) + 40;

    readimagefile("BONOL.jpg", 0, 0, 800, 600);
    nume NumeImagini[7][4];

    citesteDinFisier.open(FisiereButoaneLimba[TablaJocCurent.SetariJoc.indice_limba][1].nume);

    citesteButonDinFisierulCurentDeschis(Play);
    citesteButonDinFisierulCurentDeschis(Settings);
    citesteButonDinFisierulCurentDeschis(Instructions);
    citesteButonDinFisierulCurentDeschis(Exit);

    centreaza_buton_pe_latime(Play);
    centreaza_buton_pe_latime(Instructions);
    centreaza_buton_pe_latime(Settings);

    scaleazaSiRepozitioneazaButon(Play, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(Settings, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(Instructions, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(Exit, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    readimagefile("BONOL.jpg", 0, 0, TablaJocCurent.SetariJoc.rezolutie.x, TablaJocCurent.SetariJoc.rezolutie.y);

    deseneazaButon2(Play);
    deseneazaButon2(Settings);
    deseneazaButon2(Instructions);
    deseneazaButon2(Exit);

    bool e_in_meniu;
    e_in_meniu = 1;
    xx = 0;
    yy = 0;
    while(e_in_meniu)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, xx, yy);
            if(eCursorulPeButon(xx, yy, Play))
            {
                e_in_meniu = 0;
                eInConfiguratii = 1;
            }
            else if(eCursorulPeButon(xx, yy, Exit))
            {
                e_in_meniu = 0;
                closegraph();
                exit(0);
            }
            else if(eCursorulPeButon(xx, yy, Instructions)){
                e_in_meniu = 0;
                eInInstructiuni = 1;
            }
            else if(eCursorulPeButon(xx, yy, Settings)){
                e_in_meniu = 0;
                eInSetari = 1;
            }
        }
    }
    eInMeniu = 0;
    citesteDinFisier.close();
    return;
}

void seteazaFisiereLimba(nume FisiereButoane[7][6]){
    strcpy(FisiereButoane[1][1].nume, "Fisiere/Romana/butoane_meniu.in");
    strcpy(FisiereButoane[1][2].nume, "Fisiere/Romana/butoane_instructiuni.in");
    strcpy(FisiereButoane[1][3].nume, "Fisiere/Romana/butoane_configuratii.in");
    strcpy(FisiereButoane[1][4].nume, "Fisiere/Romana/butoane_gameplay.in");
    strcpy(FisiereButoane[1][5].nume, "Fisiere/Romana/butoane_setari.in");

    strcpy(FisiereButoane[2][1].nume, "Fisiere/Engleza/butoane_meniu.in");
    strcpy(FisiereButoane[2][2].nume, "Fisiere/Engleza/butoane_instructiuni.in");
    strcpy(FisiereButoane[2][3].nume, "Fisiere/Engleza/butoane_configuratii.in");
    strcpy(FisiereButoane[2][4].nume, "Fisiere/Engleza/butoane_gameplay.in");
    strcpy(FisiereButoane[2][5].nume, "Fisiere/Engleza/butoane_setari.in");

    strcpy(FisiereButoane[3][1].nume, "Fisiere/Franceza/butoane_meniu.in");
    strcpy(FisiereButoane[3][2].nume, "Fisiere/Franceza/butoane_instructiuni.in");
    strcpy(FisiereButoane[3][3].nume, "Fisiere/Franceza/butoane_configuratii.in");
    strcpy(FisiereButoane[3][4].nume, "Fisiere/Franceza/butoane_gameplay.in");
    strcpy(FisiereButoane[3][5].nume, "Fisiere/Franceza/butoane_setari.in");

    strcpy(FisiereButoane[4][1].nume, "Fisiere/Spaniola/butoane_meniu.in");
    strcpy(FisiereButoane[4][2].nume, "Fisiere/Spaniola/butoane_instructiuni.in");
    strcpy(FisiereButoane[4][3].nume, "Fisiere/Spaniola/butoane_configuratii.in");
    strcpy(FisiereButoane[4][4].nume, "Fisiere/Spaniola/butoane_gameplay.in");
    strcpy(FisiereButoane[4][5].nume, "Fisiere/Spaniola/butoane_setari.in");

    strcpy(FisiereButoane[5][1].nume, "Fisiere/Germana/butoane_meniu.in");
    strcpy(FisiereButoane[5][2].nume, "Fisiere/Germana/butoane_instructiuni.in");
    strcpy(FisiereButoane[5][3].nume, "Fisiere/Germana/butoane_configuratii.in");
    strcpy(FisiereButoane[5][4].nume, "Fisiere/Germana/butoane_gameplay.in");
    strcpy(FisiereButoane[5][5].nume, "Fisiere/Germana/butoane_setari.in");

    strcpy(FisiereButoane[6][1].nume, "Fisiere/Italiana/butoane_meniu.in");
    strcpy(FisiereButoane[6][2].nume, "Fisiere/Italiana/butoane_instructiuni.in");
    strcpy(FisiereButoane[6][3].nume, "Fisiere/Italiana/butoane_configuratii.in");
    strcpy(FisiereButoane[6][4].nume, "Fisiere/Italiana/butoane_gameplay.in");
    strcpy(FisiereButoane[6][5].nume, "Fisiere/Italiana/butoane_setari.in");
}

void acceseazaSetari(tabla_de_joc &TablaJocCurent){
    buton BifatPeTotEcranul, CasetaRezolutie, CasetaLimba, BifatOff, BifatOn, MesajSetari, MesajSonor, SonorOn, SonorOff, MesajLimba, LimbaPlus, LimbaMinus, MesajRezolutie, RezolutiePlus, RezolutieMinus, MesajPeTotEcranul, MesajInapoiMeniu;
    bool ok;
    int xx, yy, indice_limba;
    nume Limba[10];
    nume Rezolutii[10];
    sistem_cartezian ValoriRezolutii[10];
    ok = 1;

    citesteDinFisier.open(FisiereButoaneLimba[TablaJocCurent.SetariJoc.indice_limba][5].nume);
    strcpy(Limba[1].nume, "Romana");
    strcpy(Limba[2].nume, "English");
    strcpy(Limba[3].nume, "Francais");
    strcpy(Limba[4].nume, "Espagnol");
    strcpy(Limba[5].nume, "Deutch");
    strcpy(Limba[6].nume, "Italiano");
    strcpy(Rezolutii[1].nume, "800x600");
    strcpy(Rezolutii[2].nume, "1200x700");
    strcpy(Rezolutii[3].nume, "1360x760");
    ValoriRezolutii[1].x = 800; ValoriRezolutii[1].y = 600;
    ValoriRezolutii[2].x = 1200; ValoriRezolutii[2].y = 700;
    ValoriRezolutii[3].x = 1360; ValoriRezolutii[3].y = 760;
    ValoriRezolutii[4].x = GetSystemMetrics(SM_CXSCREEN) + 5;
    ValoriRezolutii[4].y = GetSystemMetrics(SM_CYSCREEN) + 5;

    indice_vechi = 1;

    citesteButonDinFisierulCurentDeschis(MesajSetari);
    citesteButonDinFisierulCurentDeschis(MesajSonor);
    citesteButonDinFisierulCurentDeschis(SonorOn);
    citesteButonDinFisierulCurentDeschis(BifatOn);
    citesteButonDinFisierulCurentDeschis(SonorOff);
    citesteButonDinFisierulCurentDeschis(BifatOff);
    citesteButonDinFisierulCurentDeschis(MesajLimba);
    citesteButonDinFisierulCurentDeschis(CasetaLimba);
    citesteButonDinFisierulCurentDeschis(LimbaPlus);
    citesteButonDinFisierulCurentDeschis(LimbaMinus);
    citesteButonDinFisierulCurentDeschis(MesajRezolutie);
    citesteButonDinFisierulCurentDeschis(CasetaRezolutie);
    citesteButonDinFisierulCurentDeschis(RezolutiePlus);
    citesteButonDinFisierulCurentDeschis(RezolutieMinus);
    citesteButonDinFisierulCurentDeschis(MesajPeTotEcranul);
    citesteButonDinFisierulCurentDeschis(BifatPeTotEcranul);
    citesteButonDinFisierulCurentDeschis(MesajInapoiMeniu);

    sterge_fereastra(BLACK);
    centreaza_buton_pe_latime(MesajInapoiMeniu);
    centreaza_buton_pe_latime(MesajSetari);

    BifatOn.pozitie.x = SonorOn.pozitie.x-BifatOn.pozitie.lungime-BifatOn.outline_border_width;
    BifatOn.pozitie.y = SonorOn.pozitie.y;
    BifatOff.pozitie.x = SonorOff.pozitie.x - BifatOff.pozitie.lungime-BifatOff.outline_border_width;
    BifatOff.pozitie.y = SonorOff.pozitie.y;
    LimbaMinus.pozitie.x = CasetaLimba.pozitie.x - LimbaMinus.pozitie.lungime - LimbaMinus.outline_border_width;
    LimbaMinus.pozitie.y = CasetaLimba.pozitie.y;
    LimbaPlus.pozitie.x = CasetaLimba.pozitie.x + CasetaLimba.pozitie.lungime + LimbaPlus.outline_border_width;
    LimbaPlus.pozitie.y = CasetaLimba.pozitie.y;
    RezolutiePlus.pozitie.x = CasetaRezolutie.pozitie.x + CasetaRezolutie.pozitie.lungime + RezolutiePlus.outline_border_width;
    RezolutiePlus.pozitie.y = CasetaRezolutie.pozitie.y;
    RezolutieMinus.pozitie.x = CasetaRezolutie.pozitie.x - RezolutieMinus.pozitie.lungime - RezolutieMinus.outline_border_width;
    RezolutieMinus.pozitie.y = CasetaRezolutie.pozitie.y;
    BifatPeTotEcranul.pozitie.x = MesajPeTotEcranul.pozitie.x - BifatPeTotEcranul.pozitie.lungime - BifatPeTotEcranul.outline_border_width;
    BifatPeTotEcranul.pozitie.y = MesajPeTotEcranul.pozitie.y;

    TablaJocCurent.SetariJoc.rezolutie = ValoriRezolutii[indice_rezolutie];
    if(indice_rezolutie==4){
        seteazaCuloare(BifatPeTotEcranul.background_color, COLOR(255, 0, 0));
    }
    if(TablaJocCurent.SetariJoc.sonor == 1){
        seteazaCuloare(BifatOn.background_color, COLOR(255, 0, 0));
        seteazaCuloare(BifatOff.background_color, COLOR(239, 228, 176));
    }
    else{
        seteazaCuloare(BifatOff.background_color, COLOR(255, 0, 0));
        seteazaCuloare(BifatOn.background_color, COLOR(239, 228, 176));
    }
    strcpy(Rezolutii[4].nume, MesajPeTotEcranul.nume_interior);


    scaleazaSiRepozitioneazaButon(MesajSetari, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(MesajSonor, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(SonorOn, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(BifatOn, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(SonorOff, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(BifatOff, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(MesajLimba, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(CasetaLimba, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(LimbaPlus, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(LimbaMinus, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(MesajRezolutie, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(CasetaRezolutie, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(RezolutiePlus, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(RezolutieMinus, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(MesajPeTotEcranul, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(BifatPeTotEcranul, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);
    scaleazaSiRepozitioneazaButon(MesajInapoiMeniu, TablaJocCurent.SetariJoc.rezolutie, ValoriRezolutii[1]);

    indice_limba = TablaJocCurent.SetariJoc.indice_limba;
    strcpy(CasetaRezolutie.nume_interior, Rezolutii[TablaJocCurent.SetariJoc.indice_rezolutie].nume);
    strcpy(CasetaLimba.nume_interior, Limba[indice_limba].nume);

    readimagefile("FundalSetari.jpg", 0, 0, TablaJocCurent.SetariJoc.rezolutie.x, TablaJocCurent.SetariJoc.rezolutie.y);

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
    deseneazaButon2(MesajSetari);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    deseneazaButon2(MesajSonor);
    deseneazaButon2(SonorOn);
    deseneazaButon2(BifatOn);
    deseneazaButon2(SonorOff);
    deseneazaButon2(BifatOff);
    deseneazaButon2(MesajLimba);
    deseneazaButon2(CasetaLimba);
    deseneazaButon2(LimbaMinus);
    deseneazaButon2(LimbaPlus);
    deseneazaButon2(MesajRezolutie);
    deseneazaButon2(CasetaRezolutie);
    deseneazaButon2(RezolutiePlus);
    deseneazaButon2(RezolutieMinus);
    deseneazaButon2(MesajPeTotEcranul);
    deseneazaButon2(BifatPeTotEcranul);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
    deseneazaButon2(MesajInapoiMeniu);
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

    indice_rezolutie = TablaJocCurent.SetariJoc.indice_rezolutie;
    indice_vechi = 1;
    while(ok){
        if(ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN, xx, yy);
            if(eCursorulPeButon(xx, yy, MesajInapoiMeniu)){
                ok = 0;
                eInMeniu = 1;
            }
            else if(eCursorulPeButon(xx, yy, BifatOn)){
                 TablaJocCurent.SetariJoc.sonor = 1;
                 seteazaCuloare(BifatOn.background_color, COLOR(255, 0, 0));
                 seteazaCuloare(BifatOff.background_color, COLOR(239 ,228 ,176));
                 deseneazaButon2(BifatOn);
                 deseneazaButon2(BifatOff);
                 //PlaySound(TEXT("Franz Liszt - Hungarian Rhapsody No.2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            }
            else if(eCursorulPeButon(xx, yy, BifatOff)){
                 TablaJocCurent.SetariJoc.sonor = 0;
                 seteazaCuloare(BifatOff.background_color, COLOR(255, 0, 0));
                 seteazaCuloare(BifatOn.background_color, COLOR(239 ,228 ,176));
                 deseneazaButon2(BifatOn);
                 deseneazaButon2(BifatOff);
                 //PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            }
            else if(eCursorulPeButon(xx, yy, LimbaPlus)){
                if(indice_limba < 6){
                    indice_limba++;
                }
                TablaJocCurent.SetariJoc.indice_limba = indice_limba;
                strcpy(CasetaLimba.nume_interior, Limba[indice_limba].nume);
                deseneazaButon2(CasetaLimba);
            }
            else if(eCursorulPeButon(xx, yy, LimbaMinus)){
                if(indice_limba > 1){
                    indice_limba--;
                }
                TablaJocCurent.SetariJoc.indice_limba = indice_limba;
                strcpy(CasetaLimba.nume_interior, Limba[indice_limba].nume);
                deseneazaButon2(CasetaLimba);
            }
            else if(eCursorulPeButon(xx, yy, RezolutieMinus)){
                if(indice_rezolutie == 4){
                    indice_vechi = 4;
                    indice_rezolutie = 1;
                }
                else if(indice_rezolutie > 1){
                    indice_vechi = indice_rezolutie;
                    indice_rezolutie--;
                }

                strcpy(CasetaRezolutie.nume_interior, Rezolutii[indice_rezolutie].nume);
                deseneazaButon2(CasetaRezolutie);
                seteazaCuloare(BifatPeTotEcranul.background_color, COLOR(239, 228, 176));
                closegraph();
                initwindow(ValoriRezolutii[indice_rezolutie].x, ValoriRezolutii[indice_rezolutie].y, "BonoL", 0, 0);
                readimagefile("FundalSetari.jpg", 0, 0, ValoriRezolutii[indice_rezolutie].x, ValoriRezolutii[indice_rezolutie].y);

                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
                scaleazaSiRepozitioneazaButon(MesajSetari, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajSetari);

                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

                scaleazaSiRepozitioneazaButon(MesajSonor, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajSonor);

                scaleazaSiRepozitioneazaButon(SonorOn, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(SonorOn);

                scaleazaSiRepozitioneazaButon(BifatOn, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatOn);

                scaleazaSiRepozitioneazaButon(SonorOff, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(SonorOff);

                scaleazaSiRepozitioneazaButon(BifatOff, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatOff);

                scaleazaSiRepozitioneazaButon(MesajLimba, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajLimba);

                scaleazaSiRepozitioneazaButon(CasetaLimba, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(CasetaLimba);

                scaleazaSiRepozitioneazaButon(LimbaMinus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(LimbaMinus);

                scaleazaSiRepozitioneazaButon(LimbaPlus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(LimbaPlus);

                scaleazaSiRepozitioneazaButon(MesajRezolutie, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajRezolutie);

                scaleazaSiRepozitioneazaButon(CasetaRezolutie, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(CasetaRezolutie);

                scaleazaSiRepozitioneazaButon(RezolutiePlus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(RezolutiePlus);

                scaleazaSiRepozitioneazaButon(RezolutieMinus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(RezolutieMinus);

                scaleazaSiRepozitioneazaButon(MesajPeTotEcranul, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajPeTotEcranul);

                scaleazaSiRepozitioneazaButon(BifatPeTotEcranul, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatPeTotEcranul);
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
                scaleazaSiRepozitioneazaButon(MesajInapoiMeniu, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajInapoiMeniu);
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

            }
            else if(eCursorulPeButon(xx, yy, RezolutiePlus)){
                indice_vechi = indice_rezolutie;
                if(indice_rezolutie < 3){
                    indice_rezolutie++;
                }
                if(indice_rezolutie != 4){
                    seteazaCuloare(BifatPeTotEcranul.background_color, COLOR(239, 228, 176));
                }
                else if(indice_rezolutie == 4){
                    seteazaCuloare(BifatPeTotEcranul.background_color, COLOR(255, 0, 0));
                }
                TablaJocCurent.SetariJoc.rezolutie = ValoriRezolutii[indice_rezolutie];
                strcpy(CasetaRezolutie.nume_interior, Rezolutii[indice_rezolutie].nume);
                deseneazaButon2(CasetaRezolutie);
                closegraph();
                if(indice_vechi == indice_rezolutie && indice_vechi == 4){
                    initwindow(ValoriRezolutii[indice_rezolutie].x, ValoriRezolutii[indice_rezolutie].y, "", -5, -5);
                }
                else{
                    initwindow(ValoriRezolutii[indice_rezolutie].x, ValoriRezolutii[indice_rezolutie].y, "BonoL", 0, 0);
                }
                readimagefile("FundalSetari.jpg", 0, 0, ValoriRezolutii[indice_rezolutie].x, ValoriRezolutii[indice_rezolutie].y);

                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
                scaleazaSiRepozitioneazaButon(MesajSetari, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajSetari);

                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

                scaleazaSiRepozitioneazaButon(MesajSonor, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajSonor);

                scaleazaSiRepozitioneazaButon(SonorOn, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(SonorOn);

                scaleazaSiRepozitioneazaButon(BifatOn, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatOn);

                scaleazaSiRepozitioneazaButon(SonorOff, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(SonorOff);

                scaleazaSiRepozitioneazaButon(BifatOff, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatOff);

                scaleazaSiRepozitioneazaButon(MesajLimba, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajLimba);

                scaleazaSiRepozitioneazaButon(CasetaLimba, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(CasetaLimba);

                scaleazaSiRepozitioneazaButon(LimbaMinus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(LimbaMinus);

                scaleazaSiRepozitioneazaButon(LimbaPlus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(LimbaPlus);

                scaleazaSiRepozitioneazaButon(MesajRezolutie, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajRezolutie);

                scaleazaSiRepozitioneazaButon(CasetaRezolutie, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(CasetaRezolutie);

                scaleazaSiRepozitioneazaButon(RezolutiePlus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(RezolutiePlus);

                scaleazaSiRepozitioneazaButon(RezolutieMinus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(RezolutieMinus);

                scaleazaSiRepozitioneazaButon(MesajPeTotEcranul, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajPeTotEcranul);

                scaleazaSiRepozitioneazaButon(BifatPeTotEcranul, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatPeTotEcranul);
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
                scaleazaSiRepozitioneazaButon(MesajInapoiMeniu, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajInapoiMeniu);
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

            }
            else if(eCursorulPeButon(xx, yy, BifatPeTotEcranul)){
                indice_vechi = indice_rezolutie;
                indice_rezolutie = 4;
                strcpy(CasetaRezolutie.nume_interior, Rezolutii[indice_rezolutie].nume);
                deseneazaButon2(CasetaRezolutie);

                closegraph();
                initwindow(ValoriRezolutii[indice_rezolutie].x, ValoriRezolutii[indice_rezolutie].y, "", -5, -5);
                readimagefile("FundalSetari.jpg", 0, 0, ValoriRezolutii[indice_rezolutie].x, ValoriRezolutii[indice_rezolutie].y);
                seteazaCuloare(BifatPeTotEcranul.background_color, COLOR(255, 0, 0));
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);

                scaleazaSiRepozitioneazaButon(MesajSetari, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajSetari);

                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);

                scaleazaSiRepozitioneazaButon(MesajSonor, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajSonor);

                scaleazaSiRepozitioneazaButon(SonorOn, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(SonorOn);

                scaleazaSiRepozitioneazaButon(BifatOn, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatOn);

                scaleazaSiRepozitioneazaButon(SonorOff, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(SonorOff);

                scaleazaSiRepozitioneazaButon(BifatOff, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatOff);

                scaleazaSiRepozitioneazaButon(MesajLimba, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajLimba);

                scaleazaSiRepozitioneazaButon(CasetaLimba, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(CasetaLimba);

                scaleazaSiRepozitioneazaButon(LimbaMinus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(LimbaMinus);

                scaleazaSiRepozitioneazaButon(LimbaPlus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(LimbaPlus);

                scaleazaSiRepozitioneazaButon(MesajRezolutie, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajRezolutie);

                scaleazaSiRepozitioneazaButon(CasetaRezolutie, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(CasetaRezolutie);

                scaleazaSiRepozitioneazaButon(RezolutiePlus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(RezolutiePlus);

                scaleazaSiRepozitioneazaButon(RezolutieMinus, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(RezolutieMinus);

                scaleazaSiRepozitioneazaButon(MesajPeTotEcranul, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajPeTotEcranul);

                scaleazaSiRepozitioneazaButon(BifatPeTotEcranul, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(BifatPeTotEcranul);
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
                scaleazaSiRepozitioneazaButon(MesajInapoiMeniu, ValoriRezolutii[indice_rezolutie], ValoriRezolutii[indice_vechi]);
                deseneazaButon2(MesajInapoiMeniu);
                settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
            }
        }
    }
    TablaJocCurent.SetariJoc.indice_limba = indice_limba;
    TablaJocCurent.SetariJoc.indice_rezolutie = indice_rezolutie;
    TablaJocCurent.SetariJoc.rezolutie = ValoriRezolutii[indice_rezolutie];
    eInSetari = 0;
    if(!strcmp(TablaJocCurent.numeJucator1, "")){
        strcpy(TablaJocCurent.numeJucator1, "111");
    }
    if(!strcmp(TablaJocCurent.numeJucator2, "")){
        strcpy(TablaJocCurent.numeJucator2, "222");
    }

    citesteDinFisier.close();
}

void Joaca(tabla_de_joc TablaJocCurent){
    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 3);
    buton MesajWinner, MesajCastigator, NumarMutariPosibile, MesajAlegeSaMuti, MesajAnuleaza, MesajIesire, MesajMeniu, MesajIncheieTura, MesajMutari, JucatorCareMuta1, JucatorCareMuta2, MesajAcumMuta;
    char numeMelodie[] = "Franz Liszt - Hungarian Rhapsody No.2.wav";
    sistem_cartezian RezolutieVeche, RezolutieCurenta;
    RezolutieVeche.x = 800; RezolutieVeche.y = 600;
    RezolutieCurenta = TablaJocCurent.SetariJoc.rezolutie;

    bool ok, atinse[10][10], luat_disc;
    mutari V[10005];
    int mutari_joc, xx, yy, jjj, jj, i, j, apasate, ii, iii, M[5][5], ij, tura_mutare, Mutare[5][5], Mutari[5][5];
    RGB culoare_fundal_disc, culoare_disc;
    ok = 1;
    bool apasat;
    sistem_cartezian A[20], A1[20], Piese[8][8], Piese_pe_tabla[5][5];
    buton P, T[5][5], B[5], Tabla[5][5], CopieButonTabla, CopieButonJucator1, CopieButonJucator2;


    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            TablaJocCurent.TablaNumerica[i][j] = 0;
        }
    }
    citesteDinFisier.open("matrice.in", ios::in);
    for(i = 1; i<=LUNGIME_TABLA; i++)
    {
        for(j = 1; j<=LUNGIME_TABLA; j++)
        {
            citesteDinFisier >> TablaJocCurent.TablaNumerica[i][j];
        }
    }
    for(i = 1; i<=NUMAR_BUTOANE_PIESA; i++)
    {
        citesteDinFisier >> Piese_pe_tabla[3][i].x >> Piese_pe_tabla[3][i].y;
    }
    for(i = 1; i<=NUMAR_BUTOANE_PIESA; i++)
    {
        citesteDinFisier >> Piese_pe_tabla[2][i].x >> Piese_pe_tabla[2][i].y;
    }
    citesteDinFisier.close();

    for(i = 1; i<=20; i++)
    {
        A[i].x = A[i].y = 0;
    }
    citesteDinFisier.open("butoane.in", ios::in);
    for(i = 1; i<=4; i++)
    {
        citesteButonDinFisierulCurentDeschis(B[i]);
    }
    citesteDinFisier.close();
    for(i = 1; i<=3; i++){
        scaleazaSiRepozitioneazaButon(B[i], RezolutieCurenta, RezolutieVeche);
    }
    seteazaCuloare(B[3].background_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[3].background_color));
    seteazaCuloare(B[3].outline_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[3].outline_color));
    seteazaCuloare(B[2].background_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[2].background_color));
    seteazaCuloare(B[2].outline_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[2].outline_color));
    seteazaCuloare(B[1].background_color, returneazaCuloareCaInt(TablaJocCurent.ChenarTabla.background_color));
    seteazaCuloare(B[1].outline_color, returneazaCuloareCaInt(TablaJocCurent.ChenarTabla.outline_color));
    TablaJocCurent.ChenarJucator[3] = CopieButonJucator1 = B[3];
    TablaJocCurent.ChenarTabla = CopieButonTabla = B[1];
    TablaJocCurent.ChenarJucator[2] = CopieButonJucator2 = B[2];

    TablaJocCurent.ChenarTabla.pozitie.x = (int)((100 * RezolutieCurenta.x*1.0/RezolutieVeche.x));
    TablaJocCurent.ChenarTabla.pozitie.y = (int)((RezolutieCurenta.y-4*TablaJocCurent.ChenarTabla.pozitie.latime)/2);

    for(i = 1; i<=4; i++)
    {
        for(j = 1; j<=4; j++)
        {
            T[i][j] = P;
        }
    }
    for(i = 1; i<=4; i++)
    {
        for(j = 1; j<=4; j++)
        {
            atinse[i][j] = 0;
        }
    }
    apasate = 0;
    culoare_disc.r = 0;
    culoare_disc.b = 255;
    culoare_disc.g = 96;
    culoare_fundal_disc.r = 150;
    culoare_fundal_disc.g = 234;
    culoare_fundal_disc.b = 255;

    readimagefile("FundalJoc.jpg", 0, 0, RezolutieCurenta.x, RezolutieCurenta.y);

    deseneazaTabla(TablaJocCurent.TablaNumerica, TablaJocCurent.TablaDeDesenat, TablaJocCurent.ChenarTabla, TablaJocCurent.ChenarJucator[2], TablaJocCurent.ChenarJucator[3], TablaJocCurent.culoare_disc, TablaJocCurent.culoare_outline_disc);
    citesteDinFisier.open("piese.in", ios::in);
    for(i = 1; i<=8; i++)
    {
        for(j = 1; j<=4; j++)
        {
            citesteDinFisier >> Piese[i][j].x >> Piese[i][j].y;
        }
    }
    citesteDinFisier.close();
    mutari_joc = 0;
    adaugaMutare(mutari_joc, V, TablaJocCurent.TablaNumerica);
    copiaza_matrice(TablaJocCurent.TablaNumerica, Mutari);
    TablaJocCurent.TuraJucator = 3;

    citesteDinFisier.open(FisiereButoaneLimba[TablaJocCurent.SetariJoc.indice_limba][4].nume);

    citesteButonDinFisierulCurentDeschis(MesajAcumMuta);
    citesteButonDinFisierulCurentDeschis(JucatorCareMuta1);
    citesteButonDinFisierulCurentDeschis(MesajMutari);
    citesteButonDinFisierulCurentDeschis(MesajAnuleaza);
    citesteButonDinFisierulCurentDeschis(MesajIesire);
    citesteButonDinFisierulCurentDeschis(MesajMeniu);
    citesteButonDinFisierulCurentDeschis(MesajAlegeSaMuti);
    citesteButonDinFisierulCurentDeschis(NumarMutariPosibile);
    citesteButonDinFisierulCurentDeschis(MesajCastigator);
    citesteButonDinFisierulCurentDeschis(MesajWinner);
    JucatorCareMuta2 = JucatorCareMuta1;
    strcpy(JucatorCareMuta1.nume_interior, TablaJocCurent.numeJucator1);
    strcpy(JucatorCareMuta2.nume_interior, TablaJocCurent.numeJucator2);

    seteazaCuloare(JucatorCareMuta1.background_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[3].background_color));
    seteazaCuloare(JucatorCareMuta1.outline_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[3].outline_color));

    seteazaCuloare(JucatorCareMuta2.background_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[2].background_color));
    seteazaCuloare(JucatorCareMuta2.outline_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[2].outline_color));

    convertesteIntSiSalveazaInString(mutari_posibile_jucatori(TablaJocCurent.TuraJucator, Piese, TablaJocCurent.TablaNumerica), NumarMutariPosibile.nume_interior);

    scaleazaSiRepozitioneazaButon(NumarMutariPosibile, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(NumarMutariPosibile);

    scaleazaSiRepozitioneazaButon(MesajAcumMuta, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(MesajAcumMuta);

    scaleazaSiRepozitioneazaButon(JucatorCareMuta1, RezolutieCurenta, RezolutieVeche);
    scaleazaSiRepozitioneazaButon(JucatorCareMuta2, RezolutieCurenta, RezolutieVeche);

    deseneazaButon2(JucatorCareMuta1);

    scaleazaSiRepozitioneazaButon(MesajMutari, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(MesajMutari);

    scaleazaSiRepozitioneazaButon(MesajAnuleaza, RezolutieCurenta, RezolutieVeche);

    scaleazaSiRepozitioneazaButon(MesajIesire, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(MesajIesire);

    scaleazaSiRepozitioneazaButon(MesajMeniu, RezolutieCurenta, RezolutieVeche);
    deseneazaButon2(MesajMeniu);

    scaleazaSiRepozitioneazaButon(MesajAlegeSaMuti, RezolutieCurenta, RezolutieVeche);
    MesajAlegeSaMuti.pozitie.y = TablaJocCurent.ChenarTabla.pozitie.y - MesajAlegeSaMuti.pozitie.latime - 2;
    MesajAlegeSaMuti.pozitie.x = TablaJocCurent.ChenarTabla.pozitie.x;

    deseneazaButon2(MesajAlegeSaMuti);

    scaleazaSiRepozitioneazaButon(MesajCastigator, RezolutieCurenta, RezolutieVeche);

    ok = 1;
    bool ok_mutare, gata_desenul, gata_jocul;
    ok_mutare = 0;
    gata_jocul = 1;
    while(ok){
        if(mutari_posibile_jucatori(TablaJocCurent.TuraJucator, Piese, TablaJocCurent.TablaNumerica) == 0){
            if(gata_jocul)
            {
                deseneazaButon2(MesajCastigator);
                seteazaCuloare(MesajWinner.background_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[5-TablaJocCurent.TuraJucator].background_color));
                seteazaCuloare(MesajWinner.outline_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[5-TablaJocCurent.TuraJucator].outline_color));
                if(TablaJocCurent.TuraJucator == 3){
                    strcpy(MesajWinner.nume_interior, TablaJocCurent.numeJucator2);
                }
                else{
                    strcpy(MesajWinner.nume_interior, TablaJocCurent.numeJucator1);
                }
                scaleazaSiRepozitioneazaButon(MesajWinner, RezolutieCurenta, RezolutieVeche);
                deseneazaButon2(MesajWinner);
                gata_jocul = 0;
            }
        }
        if(TablaJocCurent.TuraJucator == 3 && !gata_desenul){
            deseneazaButon2(JucatorCareMuta1);
            convertesteIntSiSalveazaInString(mutari_posibile_jucatori(TablaJocCurent.TuraJucator, Piese, TablaJocCurent.TablaNumerica), NumarMutariPosibile.nume_interior);
            scaleazaSiRepozitioneazaButon(NumarMutariPosibile, RezolutieCurenta, RezolutieVeche);
            deseneazaButon2(NumarMutariPosibile);
            gata_desenul = 1;
        }
        else if(TablaJocCurent.TuraJucator == 2 && !gata_desenul){
            convertesteIntSiSalveazaInString(mutari_posibile_jucatori(TablaJocCurent.TuraJucator, Piese, TablaJocCurent.TablaNumerica), NumarMutariPosibile.nume_interior);
            scaleazaSiRepozitioneazaButon(NumarMutariPosibile, RezolutieCurenta, RezolutieVeche);
            deseneazaButon2(NumarMutariPosibile);
            deseneazaButon2(JucatorCareMuta2);
            gata_desenul = 1;
        }
        if(ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN, xx, yy);
            if(eCursorulPeButon(xx, yy, MesajMeniu)){
                ok = 0;
                eInMeniu = 1;
            }
            else if(eCursorulPeButon(xx, yy, MesajAlegeSaMuti) && mutari_posibile_jucatori(TablaJocCurent.TuraJucator, Piese, TablaJocCurent.TablaNumerica)){
                seteazaCuloare(MesajAlegeSaMuti.background_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[TablaJocCurent.TuraJucator].background_color));
                seteazaCuloare(MesajAlegeSaMuti.outline_color, returneazaCuloareCaInt(TablaJocCurent.ChenarJucator[TablaJocCurent.TuraJucator].outline_color));
                deseneazaButon2(MesajAlegeSaMuti);
                ok_mutare = 1;
            }
            else if(eCursorulPeButon(xx, yy, MesajIesire)){
                ok = 0;
                closegraph();
                exit(0);
            }
        }
        else if(ok_mutare && mutari_posibile_jucatori(TablaJocCurent.TuraJucator, Piese, TablaJocCurent.TablaNumerica) != 0)
        {
            if(TablaJocCurent.modJoc == 1)
            {
                if(TablaJocCurent.TuraJucator == 3){
                    deseneazaButon2(JucatorCareMuta1);
                    mutaPiesaJucator(TablaJocCurent, TablaJocCurent.culoare_outline_disc, TablaJocCurent.culoare_disc, V, mutari_joc);
                }
                else if(TablaJocCurent.TuraJucator == 2){
                    deseneazaButon2(JucatorCareMuta2);
                    mutaPiesaAI(TablaJocCurent, Piese);
                    adaugaMutare(mutari_joc, V, TablaJocCurent.TablaNumerica);
                    ok_mutare = 0;
                    gata_desenul = 0;
                }
            }
            else if(TablaJocCurent.modJoc == 2)
            {
                gata_desenul = 0;
                if(TablaJocCurent.TuraJucator == 2){
                    deseneazaButon2(JucatorCareMuta2);
                }
                else{
                    deseneazaButon2(JucatorCareMuta1);
                }
                mutaPiesaJucator(TablaJocCurent, TablaJocCurent.culoare_outline_disc, TablaJocCurent.culoare_disc, V, mutari_joc);
                ok_mutare = 0;
            }
            seteazaCuloare(MesajAlegeSaMuti.background_color, COLOR(27, 20, 100));
            seteazaCuloare(MesajAlegeSaMuti.outline_color, COLOR(27, 20, 100));
            deseneazaButon2(MesajAlegeSaMuti);
        }
    }
    citesteDinFisier.close();
    eInJoc = 0;
    //getch();
}

int main()
{
    tabla_de_joc TablaJocCurent;

    seteazaFisiereLimba(FisiereButoaneLimba);

    initwindow(800, 600, "BonoL");

    settextstyle(TRIPLEX_FONT, HORIZ_DIR, 5);
    bool eInAplicatie = 1;
    eInMeniu = 1;
    TablaJocCurent.SetariJoc.rezolutie.x = 800;
    TablaJocCurent.SetariJoc.rezolutie.y = 600;
    TablaJocCurent.SetariJoc.indice_rezolutie = 1;
    TablaJocCurent.SetariJoc.indice_limba = 1;
    TablaJocCurent.SetariJoc.sonor = true;
    strcpy(TablaJocCurent.numeJucator1, " ");
    strcpy(TablaJocCurent.numeJucator2, " ");

//    PlaySound(TEXT("Franz Liszt - Hungarian Rhapsody No.2.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    while(eInAplicatie){
        if(eInMeniu){
            meniu(TablaJocCurent);
        }
        if(eInInstructiuni){
            acceseazaInstructiuni(TablaJocCurent);
        }
        if(eInJoc){
            Joaca(TablaJocCurent);
        }
        if(eInConfiguratii){
            configureaza_mod_joc(TablaJocCurent);
        }
        if(eInSetari){
            acceseazaSetari(TablaJocCurent);
        }
    }
    closegraph();
    return 0;
}
