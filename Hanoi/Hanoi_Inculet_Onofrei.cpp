#include <SDL/SDL.h>
#include <fstream>
#include <SDL/SDL_mixer.h>

using namespace std;

//Declaram variabile si fisiere

ifstream seteaza_coordonate("coordonate");

int indice_muzica = 3, contor_asteptare, linie, coloana, numar_mutari, delta_t = 1000, timp, ii, indice_greseala, indice_fundal = 1, tija_disc, numar_discuri_c, M[5][10], L[5][10], j, indice_disc, varf[4], numar_discuri, indice_personaj, i, x_mouse, y_mouse, indice, indice_meniu = 1, secunde, indice_b, asd = 1;
SDL_Surface *mesaj_final, *fundal_demo, *mesaj, *bara_demo, *cifre[5], *tunet[5], *fulger, *cloud[5], *background[5], *discuri_centrate[5], *imagine_discuri_a[5], *imagine_discuri, *imagine_tija, *discuri, *soare, *backg, *fpersonaj[10], *bpersonaj[10] , *imagine, *meniu[100], *buffer, *disc[10], *fundal, *alegere_parte[9], *iconita;
bool rezolvat = false, ruleaza_demo, event, stop = false, in_demo = false, in_joc = true, gata = 0, atins = 0, inca_merge = false, alegere_numar_discuri = false, ok = true, afisaremeniu=true, ok1=false, ok_tasta, ok_nivel=true, ruleaza_meniu = true, alegere = false, alegere_b, alegere_f;
SDL_Rect locatie_numar_meniu[5], locatie_total_meniu[5], locatie_mesaj, locatie_budist_demo, locatie_cifra_timp[5], locatie_bara_demo, taiere_cifre[15], locatie_cifra_numar[5], locatie_cifra_total[5], locatie_tunet[5], locatie_lightning[5], locatie_cloud, locatie_outside, locatie_tija[4], cursor, taiere_discuri[12], locatie_discuri_tija[5][15], locatie1_personaj, locatie2_personaj, locatie3_personaj, pozitie_discuri[10], locatie_discuri[10], pozitie_tija, coor_miscare[14], locatie_st, locatie_dr, locatie_disc[10], locatie_nori[5], taiere, locatia, jumatate1, jumatate2, coor_brahman[15], locatie_brahman[15], locatie_soare[5];
SDL_Event eveniment;

Mix_Music *muzica_fundal[6];
Mix_Chunk *sunet_fulger, *sunet_mutare;

int v[1000];
int c1[1000];
int c2[1000];
int c3[1000];
int s1,s2,s3,cn,n,total;

struct turnu
{
    int sursa;
    int destinatie;
} mutari[300];

Uint8 * key = SDL_GetKeyState(NULL);

struct bara_meniu
{
    SDL_Rect disc1;
    SDL_Rect disc2;
    SDL_Rect pozitie;
} Meniu[4], pozitie_disc;

struct buton_meniu
{
    SDL_Rect locatie_buton;
    SDL_Rect locatia_discului;
    SDL_Surface *imagine;
} butoane_meniu[5], butoane_demo[5], butoane_demo2[5];

struct disc_de_mutat
{
    SDL_Rect locatie_de_desenat;
    int raza;
} D[12];

bool coliziune(int ox, int oy, SDL_Rect obiect) // Functie cu ajutorul careia aflam daca doua obiecte s-au atins sau nu
{
    if(ox > obiect.x && ox < obiect.x + obiect.w && oy > obiect.y && oy < obiect.y + obiect.h)
        return true;
    else
        return false;
}

bool conditie_mutare(int x, int y) // Conditia de mutare de pe o tija pe alta
{
    if(y == 0)         // daca pe tija-destinatie nu exista discuri
        return true;       // putem pune un disc
    else if(x < y && y != 0)       // altfel, daca exista discuri si discul curent are raza mai mica decat primul care se afla pe tija
        return true;                // il putem aseza peste el
    else if(x > y)                     // altfel, daca discul curent are raza mai mare decat cea a primului disc de pe tija
        return false;                 // nu il putem aseza
    return false;
}

bool conditie_terminare()
{
    if(varf[3] == 9-numar_discuri || varf[2] == 9-numar_discuri)  // daca pe tija 2 sau 3 se afla exact numarul de discuri ales
        return true;                 // jocul s-a terminat
    else
        return false;           //altfel, jocul continua
    return false;
}

void muta(int n, int s,int d)    // Functia ce genereaza pozitiile de asezare pe tije pentru modul automat
{
    int m;
    if(n==1)
    {
        int k;
        if(s==1)
        {
            k = c1[s1];
            s1--;
        }
        if(s==2)
        {
            k = c2[s2];
            s2--;
        }
        if(s==3)
        {
            k = c3[s3];
            s3--;
        }
        if(d==1)
            c1[++s1]=k;
        if(d==2)
            c2[++s2]=k;
        if(d==3)
            c3[++s3]=k;

        mutari[++total].sursa = s;
        mutari[total].destinatie = d;
    }
    else
    {
        m = 6-s-d;
        muta(n-1,s,m);
        muta(1,s,d);
        muta(n-1,m,d);
    }
}

void meniu_principal()
{
   Mix_HaltMusic();                    // oprim muzica initiala
   Mix_PlayMusic(muzica_fundal[3], -1);         // pornim muzica pentru meniu
   while(ruleaza_meniu)                       // cat timp suntem in meniu
    {
        while(SDL_PollEvent(&eveniment))              // cat timp exista evenimente
        {
            switch(eveniment.type)             // le rezolvam in ordine
            {
            case SDL_QUIT:              // daca evenimentul e cel de inchidere al SDL-ului
                SDL_Quit();                      // inchidem SDL
                return ;                  // si programul
                break;
            case SDL_MOUSEMOTION:                // daca evenimentul consta in miscarea mouse-ului
                x_mouse = eveniment.motion.x;           // actualizam coordonatele cursorului
                y_mouse = eveniment.motion.y;
                break;
            case  SDL_MOUSEBUTTONDOWN:             // daca evenimentul consta in apasarea butoanelor mouse-ului
                x_mouse = eveniment.motion.x;      // actualizam coordonatele
                y_mouse = eveniment.motion.y;
                if(coliziune(x_mouse, y_mouse, Meniu[3].pozitie) && indice_meniu == 1) // daca s-a apasat pe butonul 3(IESIRE) si ne aflam pe pagina principala a meniului
                {
                    ruleaza_meniu = false;
                    alegere = false;
                    indice_personaj = 1;
                    numar_discuri = 1;
                    inca_merge = false;
                    in_joc = false;
                }
                else if(coliziune(x_mouse, y_mouse, Meniu[2].pozitie)) // daca s-a apasat butonul 2
                    indice_meniu = 2;   // mergem la INSTRUCTIUNI
                if(coliziune(x_mouse, y_mouse, pozitie_disc.disc1) && indice_meniu > 1)  // daca am dat click pe butonul INAPOI si daca inca ne aflam in meniu
                    indice_meniu--;                                     // mergem la pagina anterioara
                if(coliziune(x_mouse, y_mouse, pozitie_disc.disc2) && indice_meniu <= 4)     // daca am dat click pe butonul INAINTE si daca inca ne aflam in meniu
                    indice_meniu++;                                  // mergem la pagina urmatoare
                if(coliziune(x_mouse, y_mouse, pozitie_disc.disc2) && indice_meniu == 5)    // daca am apasat pe SA INCEPEM
                {
                    ruleaza_meniu = false;    // iesim din meniu
                    alegere = true;              // mergem la ALEGEREA PERSONAJULUI
                }
                else if(coliziune(x_mouse, y_mouse, Meniu[1].pozitie) && indice_meniu == 1) // daca am apasat pe JOACA
                {
                    ruleaza_meniu = false;   // iesim din meniu
                    alegere = true;         // mergem la ALEGEREA PERSONAJULUI
                }
                break;
            }
        }
        SDL_BlitSurface(meniu[indice_meniu], NULL, buffer, NULL);                         // desenam in buffer imaginea curenta a meniului
        for(i = 1; i<=3; i++)
            if(coliziune(x_mouse, y_mouse, Meniu[i].pozitie) && indice_meniu == 1)        // daca suntem cu mouse-ul in pozitia unui buton
            {
                SDL_BlitSurface(disc[i], NULL, buffer, &Meniu[i].disc1);            // desenam cele 2 discuri de o parte si de cealalta
                SDL_BlitSurface(disc[i], NULL, buffer, &Meniu[i].disc2);
            }
        if(coliziune(x_mouse, y_mouse, pozitie_disc.disc1) && indice_meniu > 1)          // daca suntem cu mouse-ul in meniu, la locatia primului disc
            SDL_BlitSurface(disc[4], NULL, buffer, &pozitie_disc.disc1);                    // il evidentiem
        else if(indice_meniu > 1)                                             // daca nu
            SDL_BlitSurface(disc[2], NULL, buffer, &pozitie_disc.disc1);       // il desenam pe cel simplu

        if(coliziune(x_mouse, y_mouse, pozitie_disc.disc2) && indice_meniu > 1)      // daca suntem cu mouse-ul in meniu, la locatia celui de-al doilea disc
            SDL_BlitSurface(disc[4], NULL, buffer, &pozitie_disc.disc2);             // il evidentiem
        else if(indice_meniu > 1)                                                     // daca nu
            SDL_BlitSurface(disc[2], NULL, buffer, &pozitie_disc.disc2);           // il desenam pe cel simplu

        SDL_Flip(buffer);               // afisam ce am desenat pana acum
    }
    return ;
}

void alegere_personaj()
{
    Mix_HaltMusic();                                   // oprim muzica curenta
    Mix_PlayMusic(muzica_fundal[2], -1);                  // pornim muzica specifica
    while(alegere == true)                                // cat timp ne aflam in curs de alegere al unui personaj
    {
        while(SDL_PollEvent(&eveniment))                    // cat timp exista evenimente
        {
            switch(eveniment.type)                       // le tratam in ordinea aparitiilor
            {
            case SDL_MOUSEMOTION:                        // daca am miscat cursorul
                x_mouse = eveniment.motion.x;               // actualizam coordonatele
                y_mouse = eveniment.motion.y;
                break;
            case  SDL_MOUSEBUTTONDOWN:                   // daca am dat click
                x_mouse = eveniment.motion.x;
                y_mouse = eveniment.motion.y;
                if(coliziune(x_mouse, y_mouse, locatie_nori[1]))     // daca am dat click pe primul nor
                {
                    alegere_b = true;                            // am ales un calugar
                    alegere_f = false;
                }
                else if(coliziune(x_mouse, y_mouse, locatie_nori[2])) // daca am dat click pe al doilea
                {
                    alegere_b = false;                         // am ales o calugarita
                    alegere_f = true;
                }
                break;
            }
        }
        if(coliziune(x_mouse, y_mouse, jumatate1))               // daca ne aflam cu mouse-ul in prima jumatate
        {
            SDL_BlitSurface(alegere_parte[1], NULL, buffer, NULL);           // desenam imaginea luminoasa cu calugarul
            if(coliziune(x_mouse, y_mouse, locatie_nori[1]))                // daca ne aflam cu mouse-ul in zona norului
                SDL_BlitSurface(alegere_parte[4], NULL, buffer, NULL);          // il coloram in rosu
        }
        else if(coliziune(x_mouse, y_mouse, jumatate2))                  // daca ne aflam cu mouse-ul in a doua jumatate
        {
            SDL_BlitSurface(alegere_parte[2], NULL, buffer, NULL);            // desenam imaginea luminoasa cu calugarita
            if(coliziune(x_mouse, y_mouse, locatie_nori[2]))                        // daca ne aflam cu mouse-ul in zona norului
                SDL_BlitSurface(alegere_parte[5], NULL, buffer, NULL);            // il coloram in rosu
        }
        else
            SDL_BlitSurface(alegere_parte[3], NULL, buffer, NULL);           // altfel, desenam fundalul inchis

        SDL_Flip(buffer);        // afisam

        if(alegere_b || alegere_f)  // daca am ales ceva
            alegere = false;     // iesim din ALEGEREA PERSONAJULUI
    }
    return ;
}

void alegere_brahman()
{
    indice_b = 1;              // setam indicele imaginii din animatie pe 1
    while(!indice_personaj)                // cat timp nu am ales unul dintre cei 4 MARI
    {
        while(SDL_PollEvent(&eveniment));     // cat timp exista evenimente
        {
            switch(eveniment.type)                 // le tratam in ordinea aparitiilor
            {
            case SDL_MOUSEMOTION:                    // daca am miscat cursorul
                x_mouse = eveniment.motion.x;        // ii actualizam coordonatele
                y_mouse = eveniment.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:                   // daca am apasat pe butoanele mouse-ului
                x_mouse = eveniment.motion.x;                // ii actualizam coordonatele
                y_mouse = eveniment.motion.y;
                for(i=1; i<=4; i++)                                        // pentru fiecare din cei 4 MARI
                    if(coliziune(x_mouse, y_mouse, locatie_brahman[i]))        // in cazul in care am dat click pe unul din ei
                        indice_personaj = i;               // inseamna ca am ales unul din ei -> i
                break;
            }
        }
        if(secunde % 10 == 0)         // temporizator -> Daca a trecut un anumit interval de timp
            indice_b++;           // animatia cu inchinaciunea ruleaza
        if(secunde == 1000)             // daca s-a ajuns la o anumita valoare
            secunde = 0;                  // setam timpul pe 0
        if(indice_b == 12)             // daca s-a ajuns la finalul animatiei
            indice_b = 1;                 // o luam de la capat
        secunde++;                    // crestem timpul
        SDL_BlitSurface(backg, NULL, buffer, NULL);          // desenam fundalul
        for(i = 1; i<=4; i++)                                // pentru fiecare din cei 4 MARI
        {
            if(alegere_b)                                         // daca am ales calugar
                SDL_BlitSurface(bpersonaj[i], &coor_brahman[indice_b], buffer, &locatie_brahman[i]);  // desenam cei 4 calugari
            else if(alegere_f)                          // daca am ales calugarite
                SDL_BlitSurface(fpersonaj[i], &coor_brahman[indice_b], buffer, &locatie_brahman[i]); // desenam cele 4 calugarite
        }
        for(i=1; i<=4; i++)                            // in cazul in care ne aflam cu mouse-ul in zona unui calugar
        {
            if(coliziune(x_mouse, y_mouse, locatie_brahman[i]))
                SDL_BlitSurface(soare, NULL, buffer, &locatie_soare[i]);     // desenam soarele de deasupra lui
        }
        SDL_Flip(buffer);                                // afisam ce am desenat
        if(indice_personaj)                           // daca am ales ceva
            alegere_numar_discuri = true;               // mergem mai departe la ALEGEREA NUMARULUI DE DISCURI
    }
    return ;
}

void citeste_coordonate()          // Functie pentru a citi coordonatele locatiilor si ale taierilor
{
    for(i = 1; i<=3; i++)
        seteaza_coordonate >> Meniu[i].pozitie.x >> Meniu[i].pozitie.y >> Meniu[i].pozitie.w >> Meniu[i].pozitie.h;
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> Meniu[i].disc1.x >> Meniu[i].disc1.y;
        seteaza_coordonate >> Meniu[i].disc2.x >> Meniu[i].disc2.y;
    }
    seteaza_coordonate >> pozitie_disc.disc1.x >> pozitie_disc.disc1.y >> pozitie_disc.disc1.w >> pozitie_disc.disc1.h;
    seteaza_coordonate >> pozitie_disc.disc2.x >> pozitie_disc.disc2.y >> pozitie_disc.disc2.w >> pozitie_disc.disc2.h;
    seteaza_coordonate >> jumatate1.x >> jumatate1.y >> jumatate1.w >> jumatate1.h;
    seteaza_coordonate >> jumatate2.x >> jumatate2.y >> jumatate2.w >> jumatate2.h;
    seteaza_coordonate >> coor_brahman[1].x >> coor_brahman[1].y >> coor_brahman[1].w >> coor_brahman[1].h;
    for(i = 2; i<=12; i++)
    {
        coor_brahman[i].w = coor_brahman[i-1].w;
        coor_brahman[i].h = coor_brahman[i-1].h;
        coor_brahman[i].y = coor_brahman[i-1].y;
        coor_brahman[i].x = coor_brahman[i-1].x + 200;
    }
    for(i = 1; i<=4; i++)
    {
        seteaza_coordonate >> locatie_brahman[i].x;
        locatie_brahman[i].y = 300;
    }
    for(i = 1; i<=4; i++)
    {
        locatie_soare[i].x = locatie_brahman[i].x - 40;
        locatie_soare[i].y = 0;
        locatie_soare[i].w = 266;
        locatie_soare[i].h = 179;
    }
    for(i = 1; i<=2; i++)
        seteaza_coordonate >> locatie_nori[i].x >> locatie_nori[i].y >> locatie_nori[i].w >> locatie_nori[i].h;
    for(i = 3; i<=8; i++)
    {
        seteaza_coordonate >> locatie_disc[i].x;
        locatie_disc[i].y = 413;
    }
    seteaza_coordonate >> locatie_st.x >> locatie_dr.x;
    locatie_st.y = locatie_dr.y = 295;

    seteaza_coordonate >> coor_miscare[1].x >> coor_miscare[1].y >> coor_miscare[1].w >> coor_miscare[1].h;
    for(i = 2; i<=12; i++)
    {
        coor_miscare[i].x = coor_miscare[i-1].x + 228;
        coor_miscare[i].y = 580;
        coor_miscare[i].w = 211;
        coor_miscare[i].h = 434;
    }
    seteaza_coordonate >> pozitie_tija.x >> pozitie_tija.y;
    seteaza_coordonate >> locatie1_personaj.x >> locatie2_personaj.x >> locatie3_personaj.x;
    locatie1_personaj.y = locatie2_personaj.y = locatie3_personaj.y = 270;

    seteaza_coordonate >> locatie_discuri_tija[1][8].x >> locatie_discuri_tija[2][8].x >> locatie_discuri_tija[3][8].x;
    locatie_discuri_tija[1][8].y = locatie_discuri_tija[2][8].y = locatie_discuri_tija[3][8].y = 602;
    locatie_discuri_tija[1][8].w = locatie_discuri_tija[2][8].w = locatie_discuri_tija[3][8].w = 454;
    locatie_discuri_tija[1][8].h = locatie_discuri_tija[2][8].h = locatie_discuri_tija[3][8].h = 49;
    for(i = 7; i>=1; i--)
    {
        locatie_discuri_tija[1][i].x = locatie_discuri_tija[1][i+1].x;
        locatie_discuri_tija[2][i].x = locatie_discuri_tija[2][i+1].x;
        locatie_discuri_tija[3][i].x = locatie_discuri_tija[3][i+1].x;

        locatie_discuri_tija[1][i].y = locatie_discuri_tija[1][i+1].y - 44;
        locatie_discuri_tija[2][i].y = locatie_discuri_tija[2][i+1].y - 44;
        locatie_discuri_tija[3][i].y = locatie_discuri_tija[3][i+1].y - 44;

        locatie_discuri_tija[1][i].w = locatie_discuri_tija[2][i].w = locatie_discuri_tija[3][i].w = locatie_discuri_tija[1][8].w;
        locatie_discuri_tija[1][i].h = locatie_discuri_tija[2][i].h = locatie_discuri_tija[3][i].h = locatie_discuri_tija[1][8].h;
    }
    seteaza_coordonate >> taiere_discuri[1].x >> taiere_discuri[1].y >> taiere_discuri[1].w >> taiere_discuri[1].h;
    for(i = 2; i<=8; i++)
    {
        taiere_discuri[i].x = taiere_discuri[i-1].x;
        taiere_discuri[i].y = taiere_discuri[i-1].y + 54;
        taiere_discuri[i].w = taiere_discuri[i-1].w;
        taiere_discuri[i].h = taiere_discuri[i-1].h;
    }
    for(i = 1; i<=3; i++)
        seteaza_coordonate >> locatie_tija[i].x >> locatie_tija[i].y >> locatie_tija[i].w >> locatie_tija[i].h;
    seteaza_coordonate >> locatie_outside.x >> locatie_outside.y;
    seteaza_coordonate >> locatie_cloud.x >> locatie_cloud.y;
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> locatie_lightning[i].x;
        locatie_lightning[i].y = 0;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> butoane_meniu[i].locatie_buton.x;
        butoane_meniu[i].locatie_buton.y = 683;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> butoane_meniu[i].locatia_discului.x;
        butoane_meniu[i].locatia_discului.y = 680;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> locatie_tunet[i].x;
        locatie_tunet[i].y = 0;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> butoane_demo[i].locatie_buton.x;
        seteaza_coordonate >> butoane_demo[i].locatia_discului.x;
        butoane_demo[i].locatia_discului.y = butoane_demo[i].locatie_buton.y = 681;
    }
    seteaza_coordonate >> taiere_cifre[0].x >> taiere_cifre[0].y >> taiere_cifre[0].w >> taiere_cifre[0].h;
    for(i = 1; i<=10; i++)
    {
        taiere_cifre[i].x = taiere_cifre[i-1].x + 91;
        taiere_cifre[i].y = taiere_cifre[i-1].y;
        taiere_cifre[i].w = taiere_cifre[i-1].w;
        taiere_cifre[i].h = taiere_cifre[i-1].h;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> locatie_cifra_numar[i].x;
        locatie_cifra_numar[i].y = 685;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> locatie_cifra_total[i].x;
        locatie_cifra_total[i].y = 685;
    }
    seteaza_coordonate >> locatie_bara_demo.x >> locatie_bara_demo.y;
    seteaza_coordonate >> locatie_budist_demo.x >> locatie_budist_demo.y;
    seteaza_coordonate >> locatie_mesaj.x >> locatie_mesaj.y;
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> locatie_cifra_timp[i].x;
        locatie_cifra_timp[i].y = 154;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> locatie_numar_meniu[i].x;
        locatie_numar_meniu[i].y = 41;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> locatie_total_meniu[i].x;
        locatie_total_meniu[i].y = 29;
    }
    for(i = 1; i<=3; i++)
    {
        seteaza_coordonate >> butoane_demo2[i].locatie_buton.x >> butoane_demo2[i].locatia_discului.x;
        butoane_demo2[i].locatie_buton.y = butoane_demo2[i].locatia_discului.y = 682;
    }

    for(i=1; i<=n; i++)
        c1[i]=i;
}

void incarca_imagini()                 // Functie pentru incarcarea imaginilor si a initierii modulelor SDL etc.
{
    SDL_Init( SDL_INIT_EVERYTHING );                 // Initializam toate modulele
    buffer = SDL_SetVideoMode(1366, 768, 32, SDL_SWSURFACE | SDL_FULLSCREEN);        // cream o fereastra FULLSCREEN
    SDL_WM_SetCaption("Turnurile din Hanoi", NULL);                       // setam numele ferestrei
    iconita = SDL_LoadBMP("./Imagini/Icon.bmp");                        // Incarcam imaginea iconitei din bara
    SDL_WM_SetIcon(iconita, NULL);                                   // Setam imaginea iconitei
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);          // Initializam modul audio

    muzica_fundal[1] = Mix_LoadMUS("./Sonor/Kevin MacLeod - Smoking Gun.mp3");       // incarcam muzica
    muzica_fundal[2] = Mix_LoadMUS("./Sonor/Kevin MacLeod - Exciting Trailer.mp3");
    muzica_fundal[3] = Mix_LoadMUS("./Sonor/Kevin MacLeod - Moonlight Hall.mp3");
    muzica_fundal[4] = Mix_LoadMUS("./Sonor/Kevin MacLeod - There It Is.mp3");

    sunet_fulger = Mix_LoadWAV("./Sonor/Lightning.wav");
    sunet_mutare = Mix_LoadWAV("./Sonor/Mutare.wav");

    meniu[1] = SDL_LoadBMP("./Imagini/Meniu1.bmp");                   // incarcam imagini
    meniu[2] = SDL_LoadBMP("./Imagini/Instructiuni1.bmp");
    meniu[3] = SDL_LoadBMP("./Imagini/Instructiuni2.bmp");
    meniu[4] = SDL_LoadBMP("./Imagini/Instructiuni3.bmp");

    disc[1] = SDL_LoadBMP("./Imagini/disc1.bmp");
    disc[2] = SDL_LoadBMP("./Imagini/disc2.bmp");
    disc[3] = SDL_LoadBMP("./Imagini/disc3.bmp");
    disc[4] = SDL_LoadBMP("./Imagini/disc4.bmp");

    Uint32 colorkey = SDL_MapRGB(disc[1]->format, 0xFF, 0, 0xFF);   // setam codul hexa unei variabile-culori si il transformam in 0

    alegere_parte[1] = SDL_LoadBMP("./Imagini/Alegere1.bmp");
    alegere_parte[2] = SDL_LoadBMP("./Imagini/Alegere2.bmp");
    alegere_parte[3] = SDL_LoadBMP("./Imagini/Alegere3.bmp");
    alegere_parte[4] = SDL_LoadBMP("./Imagini/Alegere11.bmp");
    alegere_parte[5] = SDL_LoadBMP("./Imagini/Alegere22.bmp");

    fpersonaj[1] = SDL_LoadBMP("./Personaje/FAlb.bmp");
    fpersonaj[2] = SDL_LoadBMP("./Personaje/FAlbastru.bmp");
    fpersonaj[3] = SDL_LoadBMP("./Personaje/FRosu.bmp");
    fpersonaj[4] = SDL_LoadBMP("./Personaje/FClassic.bmp");
    bpersonaj[1] = SDL_LoadBMP("./Personaje/BAlb.bmp");
    bpersonaj[2] = SDL_LoadBMP("./Personaje/BAlbastru.bmp");
    bpersonaj[3] = SDL_LoadBMP("./Personaje/BRosu.bmp");
    bpersonaj[4] = SDL_LoadBMP("./Personaje/BClassic.bmp");

    imagine_discuri = SDL_LoadBMP("./Imagini/Discurile.bmp");
    imagine_discuri_a[1] = SDL_LoadBMP("./Imagini/Discurile1.bmp");
    imagine_discuri_a[2] = SDL_LoadBMP("./Imagini/Discurile2.bmp");
    imagine_discuri_a[3] = SDL_LoadBMP("./Imagini/Discurile3.bmp");
    imagine_discuri_a[4] = SDL_LoadBMP("./Imagini/Discurile4.bmp");

    soare = SDL_LoadBMP("./Imagini/Soare.bmp");
    fulger = SDL_LoadBMP("./Imagini/Fulger.bmp");
    discuri_centrate[1] = SDL_LoadBMP("./Imagini/Discurile_centrate_Alb.bmp");
    discuri_centrate[2] = SDL_LoadBMP("./Imagini/Discurile_centrate_Albastru.bmp");
    discuri_centrate[3] = SDL_LoadBMP("./Imagini/Discurile_centrate_Rosu.bmp");
    discuri_centrate[4] = SDL_LoadBMP("./Imagini/Discurile_centrate_Classic.bmp");

    for(i = 1; i<=4; i++)                                                      // curatam imaginile de fundalul MAGENTA
    {
        SDL_SetColorKey(bpersonaj[i], SDL_SRCCOLORKEY, colorkey);
        SDL_SetColorKey(fpersonaj[i], SDL_SRCCOLORKEY, colorkey);
    }
    for(i = 1; i<=4; i++)
        SDL_SetColorKey(disc[i], SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(soare, SDL_SRCCOLORKEY, colorkey);

    background[1] = SDL_LoadBMP("./Imagini/Background1.bmp");
    background[2] = SDL_LoadBMP("./Imagini/Background2.bmp");
    cloud[1] = SDL_LoadBMP("./Imagini/Nor1.bmp");
    cloud[2] = SDL_LoadBMP("./Imagini/Nor2.bmp");

    backg = SDL_LoadBMP("./Imagini/Back.bmp");
    discuri = SDL_LoadBMP("./Imagini/Alegere_Discuri.bmp");
    imagine_tija = SDL_LoadBMP("./Imagini/Tije.bmp");
    butoane_meniu[1].imagine = SDL_LoadBMP("./Imagini/Buton1.bmp");
    butoane_meniu[2].imagine = SDL_LoadBMP("./Imagini/Buton2.bmp");
    butoane_meniu[3].imagine = SDL_LoadBMP("./Imagini/Buton3.bmp");
    tunet[1] = SDL_LoadBMP("./Imagini/Thunder1.bmp");
    tunet[2] = SDL_LoadBMP("./Imagini/Thunder2.bmp");
    tunet[3] = SDL_LoadBMP("./Imagini/Thunder3.bmp");

    butoane_demo[1].imagine = SDL_LoadBMP("./Imagini/Buton4.bmp");
    butoane_demo[2].imagine = SDL_LoadBMP("./Imagini/Buton5.bmp");
    butoane_demo[3].imagine = SDL_LoadBMP("./Imagini/Buton6.bmp");

    butoane_demo2[1].imagine = SDL_LoadBMP("./Imagini/Buton1.bmp");
    butoane_demo2[2].imagine = SDL_LoadBMP("./Imagini/Buton7.bmp");
    butoane_demo2[3].imagine = SDL_LoadBMP("./Imagini/Buton2.bmp");

    cifre[1] = SDL_LoadBMP("./Imagini/Cifre_Alb.bmp");
    cifre[2] = SDL_LoadBMP("./Imagini/Cifre_Albastru.bmp");
    cifre[3] = SDL_LoadBMP("./Imagini/Cifre_Rosu.bmp");
    cifre[4] = SDL_LoadBMP("./Imagini/Cifre_Classic.bmp");
    bara_demo = SDL_LoadBMP("./Imagini/Bara_demo.bmp");
    mesaj = SDL_LoadBMP("./Imagini/Mesaj.bmp");
    mesaj_final = SDL_LoadBMP("./Imagini/Mesaj_final.bmp");
    fundal_demo = SDL_LoadBMP("./Imagini/Background_Demo.bmp");

    SDL_SetColorKey(imagine_tija, SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(imagine_discuri, SDL_SRCCOLORKEY, colorkey);
    for(i = 1; i<=4; i++)
        SDL_SetColorKey(imagine_discuri_a[i], SDL_SRCCOLORKEY, colorkey);
    for(i = 1; i<=4; i++)
        SDL_SetColorKey(discuri_centrate[i], SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(fulger, SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(cloud[1], SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(bara_demo, SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(mesaj, SDL_SRCCOLORKEY, colorkey);
    SDL_SetColorKey(mesaj_final, SDL_SRCCOLORKEY, colorkey);

    for(i=1; i<=4; i++)
        SDL_SetColorKey(cifre[i], SDL_SRCCOLORKEY, colorkey);

    Mix_VolumeMusic(128);                                // setam volumul muzicii -> MAXIM
    Mix_VolumeChunk(sunet_fulger, 128);           // setam volumul efectului de fulger => MAXIM
    Mix_VolumeChunk(sunet_mutare, 20);              // setam volumul efectului de miscare
}

void alegere_discuri()
{
    while(alegere_numar_discuri && !numar_discuri)           // cat timp nu am ales numarul discurilor
    {
        while(SDL_PollEvent(&eveniment))                        // cat timp exista evenimente
        {
            switch(eveniment.type)                 // le tratam in ordinea in care apar
            {
            case SDL_MOUSEMOTION:                      // in cazul in care am miscat mouse-ul
                x_mouse = eveniment.motion.x;               // ii actualizam pozitia
                y_mouse = eveniment.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:                           // daca am dat click
                for(i = 3; i<=8; i++)                            // daca am dat click in zona unui disc
                    if(coliziune(x_mouse, y_mouse, locatie_disc[i]))
                    {
                        numar_discuri = i;                 // am ales numarul de discuri -> i
                        inca_merge = true;                    // putem incepe JOCUL
                    }
                break;
            }
        }
        SDL_BlitSurface(discuri, NULL, buffer, NULL);           // desenam fundalul
        if(alegere_b == true)                               // daca am ales calugari
        {
            SDL_BlitSurface(bpersonaj[indice_personaj], &coor_miscare[4], buffer, &locatie_st);  // desenam in stanga si in dreapta cei 2 calugari
            SDL_BlitSurface(bpersonaj[indice_personaj], &coor_miscare[10], buffer, &locatie_dr);
        }
        else if(alegere_f == true)    // daca am ales calugarite
        {
            SDL_BlitSurface(fpersonaj[indice_personaj], &coor_miscare[4], buffer, &locatie_st); // desenam in stanga si in dreapta cele 2 calugarite
            SDL_BlitSurface(fpersonaj[indice_personaj], &coor_miscare[10], buffer, &locatie_dr);
        }
        for(i = 3; i<=8; i++)                                     // daca ne aflam in zona vreunuia din cele 6 discuri
            if(coliziune(x_mouse, y_mouse, locatie_disc[i]))
                SDL_BlitSurface(disc[4], NULL, buffer, &locatie_disc[i]);         // evidentiem discul respectiv
            else         // daca nu
                SDL_BlitSurface(disc[2], NULL, buffer, &locatie_disc[i]);         // il desenam simplu
        SDL_Flip(buffer);     // afisam tot
    }
    return ;
}

void joaca()                        // Functia JOC
{
    Mix_HaltMusic();                                     // oprim muzica curenta
    Mix_PlayMusic(muzica_fundal[1], -1);                    // pornim muzica respectiva
    locatie_cloud.x = 1366;                       // setam locatia norului
    s1 = numar_discuri;
    cn = numar_discuri;
    total = 0;
    muta(numar_discuri,1,3);            // generam pozitiile pentru modul AUTOMAT
    varf[1] = 9-numar_discuri;                 // setam baza primei tije pe 9-numarul_discurilor
    varf[2] = 9;                                  // noi punem discurile de jos in sus (daca am fi ales un disc, l-am fi pus la baza, adica pe pozitia 8)
    varf[3] = 9;                               // celelalte 2 tije sunt goale
    for(j = 9-numar_discuri; j<=8; j++)          // setam in 2 matrici numerice indicele discului pentru desen si locatie
    {
        M[1][j] = j;
        L[1][j] = j;
    }
    numar_mutari = 0;     // setam numarul de mutari pe 0
    while(inca_merge)                    // cat timp jucam
    {
        if(conditie_terminare())            // daca am terminat
        {
            Mix_HaltMusic();                                // oprim muzica curenta
            Mix_PlayMusic(muzica_fundal[4], -1);             // pornim muzica respectiva
            inca_merge = false;                       // iesim din modul de joc
            rezolvat = true;                    //  inseamna ca am rezolvat cu succes jocul
        }
        SDL_BlitSurface(background[indice_fundal], NULL, buffer, NULL);            // desenam imaginea de fundal

        locatie_cloud.x--;                                            // mutam norul mai la stanga
        if(locatie_cloud.x == 0)                         // daca a ajuns la capatul ecranului
            locatie_cloud.x = 1366;             // o ia de la inceput
        if(secunde >= 10000)                // daca a trecut prea mult timp
            secunde = 0;                    // setam temporizatorul pe 0
        secunde++;                         // crestem numarul de secunde

        if(indice_fundal == 1)               // daca nu am facut o miscare gresita
        {
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari%10], buffer, &locatie_numar_meniu[3]);   // desenam numarul de mutari
            if(numar_mutari < 10)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_numar_meniu[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_numar_meniu[2]);
            }
            else if(numar_mutari >= 10 && numar_mutari < 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_numar_meniu[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari/10%10], buffer, &locatie_numar_meniu[2]);
            }
            else if(numar_mutari >= 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari/100], buffer, &locatie_numar_meniu[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari/10%10], buffer, &locatie_numar_meniu[2]);
            }
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total%10], buffer, &locatie_total_meniu[3]);         // desenam numarul de cifre pentru totalul numarului de mutari
            if(total < 10)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_total_meniu[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_total_meniu[2]);
            }
            else if(total >= 10 && total < 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_total_meniu[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/10%10], buffer, &locatie_total_meniu[2]);
            }
            else if(total >= 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/100], buffer, &locatie_total_meniu[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/10%10], buffer, &locatie_total_meniu[2]);
            }
        }
        if(indice_fundal == 1)                                                                    // daca nu am efectuat o miscare gresita
            SDL_BlitSurface(cloud[indice_fundal], NULL, buffer, &locatie_cloud);               // desenam norul
        if(indice_fundal == 2 && secunde % 4 == 0)                                          // daca am gresit si intervalul de timp e ok
            SDL_BlitSurface(tunet[1], NULL, buffer, &locatie_tunet[1]);                       // desenam tunetele
        if(indice_fundal == 2 && secunde % 3 == 0)
            SDL_BlitSurface(tunet[2], NULL, buffer, &locatie_tunet[2]);
        if(indice_fundal == 2 && secunde % 5 == 0)
            SDL_BlitSurface(tunet[3], NULL, buffer, &locatie_tunet[3]);

        if(alegere_b)                                                                        // daca am ales calugari
            SDL_BlitSurface(bpersonaj[indice_personaj], &coor_miscare[4], buffer, &locatie1_personaj);    // desenam calugarul
        else if(alegere_f)                              // daca am ales calugarite
            SDL_BlitSurface(fpersonaj[indice_personaj], &coor_miscare[4], buffer, &locatie1_personaj);  // desenam calugarita
        SDL_BlitSurface(imagine_tija, NULL, buffer, &pozitie_tija);     // desenam tija
        for(i = 1; i<=3; i++)                                                     // desenam butoanele meniului
            SDL_BlitSurface(butoane_meniu[i].imagine, NULL, buffer, &butoane_meniu[i].locatie_buton);
        for(i = 1; i<=3; i++)                                       // verificam daca ne aflam cu mouse-ul pe un buton din meniu
        {
            if(coliziune(x_mouse, y_mouse, butoane_meniu[i].locatie_buton) && !atins)
                SDL_BlitSurface(disc[4], NULL, buffer, &butoane_meniu[i].locatia_discului);   // daca da, evidentiem acel disc
            else
                SDL_BlitSurface(disc[2], NULL, buffer, &butoane_meniu[i].locatia_discului);    // daca nu, il desenam simplu
        }

        for(i = 1; i<=3; i++)               // desenam discurile
        {
            for(j = varf[i]; j<=8; j++)
            {
                if(M[i][j] != 0 && L[i][j] != 0)
                    SDL_BlitSurface(imagine_discuri, &taiere_discuri[M[i][j]], buffer, &locatie_discuri_tija[i][L[i][j]]);
            }
        }
        while(SDL_PollEvent(&eveniment))            // cat timp exista evenimente
        {
            switch(eveniment.type)                       // le tratam in ordinea in care apar
            {
            case SDL_MOUSEMOTION:                                // daca am miscat cursorul
                x_mouse = cursor.x = eveniment.motion.x;                  // actualizam coordonatele
                y_mouse = cursor.y = eveniment.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:                            // daca am dat click
                x_mouse = cursor.x = eveniment.motion.x;            // actualizam coordonatele
                y_mouse = cursor.y = eveniment.motion.y;
                if(coliziune(x_mouse, y_mouse, butoane_meniu[2].locatie_buton) && !atins)      // daca am dat click pe butonul 2 -> IESIRE si nu avem disc in mana
                {
                    inca_merge = false;         // iesim din modul de JOC
                    in_joc = false;     // iesim din joc
                }
                if(coliziune(x_mouse, y_mouse, butoane_meniu[1].locatie_buton) && !atins) // daca am dat click pe butonul 1 -> MENIU si nu avem disc in mana
                {
                    inca_merge = false;    // setam toate variabilele cu valori opuse pentru a ajunge la meniu si pentru a se putea inlantui in ordinea initiala
                    ruleaza_meniu = true;
                    alegere = true;
                    alegere_b = false;
                    alegere_f = false;
                    indice_personaj = 0;
                    numar_discuri = 0;
                }
                if(coliziune(x_mouse, y_mouse, butoane_meniu[3].locatie_buton) && !atins)         // daca am dat click pe butonul 3 -> REZOLVARE si nu avem discuri in mana
                {
                    inca_merge = false;                 // iesim din modul JOC
                    in_demo = true;                  // mergem mai departe la DEMO
                }

                for(i = 1; i<=3; i++)                      // verificam daca ne aflam in zona unei tije
                {
                    if(coliziune(x_mouse, y_mouse, locatie_tija[i]))            // daca da
                    {
                        if(!atins)                            // daca nu avem discuri in mana
                        {
                            indice_disc = M[i][varf[i]];          // retinem discul din varful tijei
                            linie = i;                            // retinem coordonatele sale in functie de tija
                            coloana = varf[i];
                            M[i][varf[i]] = 0;             // ii resetam pozitia
                            L[i][varf[i]] = 0;
                            varf[i]++;                     // mergem in josul tijei cu o pozitie
                            atins = 1;                     // am luat discul
                            Mix_PlayChannel(-1, sunet_mutare, 0);        // redam sunetul unei mutari
                        }
                        else if(atins && conditie_mutare(indice_disc, M[i][varf[i]]))   // daca avem disc si respectam conditia de mutare
                        {
                            varf[i]--;                                  // mergem in susul tijei cu o pozitie
                            if(linie != i && coloana != M[i][varf[i]])              // daca pozitia initiala nu coindice cu cea curenta
                                numar_mutari++;                       // inseamna ca am facut o mutare
                            M[i][varf[i]] = indice_disc;           // retinem discul si indicele sau de pozitie
                            L[i][varf[i]] = varf[i];
                            atins = 0;                        // am pus discul
                            indice_fundal = 1;                    // nu am gresit
                            Mix_PlayChannel(-1, sunet_mutare, 0);          // redam sunetul de mutare
                        }
                        else if(atins && !conditie_mutare(indice_disc, M[i][varf[i]]))    // daca avem disc si nu am respectat conditia
                        {
                            indice_fundal = 2;                         // setam indicele fundalului pe 2 -> Furtuna
                            indice_greseala = i;                          // retinem tija unde doream sa mutam discul
                            Mix_PlayChannel(-1, sunet_fulger, 0);              // redam sunetul de fulger
                        }
                    }
                }
                break;
            }
        }
        if(indice_fundal == 2 && secunde % 10 == 0)                            // daca am gresit si timpul e ok
            SDL_BlitSurface(fulger, NULL, buffer, &locatie_lightning[indice_greseala]);         // desenam fulgerul
        if(atins)                                    // daca avem disc in mana
            SDL_BlitSurface(discuri_centrate[indice_personaj], &taiere_discuri[indice_disc], buffer, &cursor);  // il desenam la pozitia cursorului
        for(i = 1; i<=3; i++)
            if(coliziune(x_mouse, y_mouse, locatie_discuri_tija[i][varf[i]]) && !atins)                  // daca ne aflam cu mouse-ul pe discul din varful tijei
                SDL_BlitSurface(imagine_discuri_a[indice_personaj], &taiere_discuri[M[i][varf[i]]], buffer, &locatie_discuri_tija[i][varf[i]]);       // il desenam in functie de culoarea calugarului
        SDL_Flip(buffer);    // afisam tot ce am desenat
    }
    while(rezolvat == true)             // cat timp ne aflam in demo
    {
        while(SDL_PollEvent(&eveniment))
        {
            switch(eveniment.type)
            {
            case SDL_MOUSEMOTION:
                x_mouse = eveniment.motion.x;
                y_mouse = eveniment.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                x_mouse = eveniment.motion.x;
                y_mouse = eveniment.motion.y;
                if(coliziune(x_mouse, y_mouse, butoane_meniu[2].locatie_buton))
                {
                    inca_merge = false;
                    in_joc = false;
                    rezolvat = false;
                }
                if(coliziune(x_mouse, y_mouse, butoane_meniu[1].locatie_buton))
                {
                    inca_merge = false;
                    ruleaza_meniu = true;
                    alegere = true;
                    alegere_b = false;
                    alegere_f = false;
                    indice_personaj = 0;
                    numar_discuri = 0;
                    rezolvat = false;
                }
                if(coliziune(x_mouse, y_mouse, butoane_meniu[3].locatie_buton))
                {
                    inca_merge = false;
                    in_demo = true;
                    rezolvat = false;
                }
                break;
            }
        }
        SDL_BlitSurface(background[1], NULL, buffer, NULL);

        if(alegere_b)
            SDL_BlitSurface(bpersonaj[indice_personaj], &coor_miscare[7], buffer, &locatie_budist_demo);
        else if(alegere_f)
            SDL_BlitSurface(fpersonaj[indice_personaj], &coor_miscare[7], buffer, &locatie_budist_demo);
        SDL_BlitSurface(imagine_tija, NULL, buffer, &pozitie_tija);

        SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari%10], buffer, &locatie_numar_meniu[3]);
        if(numar_mutari < 10)
        {
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_numar_meniu[1]);
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_numar_meniu[2]);
        }
        else if(numar_mutari >= 10 && numar_mutari < 100)
        {
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_numar_meniu[1]);
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari/10%10], buffer, &locatie_numar_meniu[2]);
        }
        else if(numar_mutari >= 100)
        {
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari/100], buffer, &locatie_numar_meniu[1]);
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[numar_mutari/10%10], buffer, &locatie_numar_meniu[2]);
        }
        SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total%10], buffer, &locatie_total_meniu[3]);
        if(total < 10)
        {
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_total_meniu[1]);
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_total_meniu[2]);
        }
        else if(total >= 10 && total < 100)
        {
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_total_meniu[1]);
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/10%10], buffer, &locatie_total_meniu[2]);
        }
        else if(total >= 100)
        {
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/100], buffer, &locatie_total_meniu[1]);
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/10%10], buffer, &locatie_total_meniu[2]);
        }
        SDL_BlitSurface(mesaj_final, NULL, buffer, &locatie_mesaj);
        for(i = 1; i<=3; i++)
        {
            for(j = varf[i]; j<=8; j++)
            {
                if(M[i][j] != 0 && L[i][j] != 0)
                    SDL_BlitSurface(imagine_discuri, &taiere_discuri[M[i][j]], buffer, &locatie_discuri_tija[i][L[i][j]]);
            }
        }
        for(i = 1; i<=3; i++)
            SDL_BlitSurface(butoane_meniu[i].imagine, NULL, buffer, &butoane_meniu[i].locatie_buton);
        for(i = 1; i<=3; i++)
        {
            if(coliziune(x_mouse, y_mouse, butoane_meniu[i].locatie_buton))
                SDL_BlitSurface(disc[4], NULL, buffer, &butoane_meniu[i].locatia_discului);
            else
                SDL_BlitSurface(disc[2], NULL, buffer, &butoane_meniu[i].locatia_discului);
        }
        SDL_Flip(buffer);
    }
}

void mod_automat()
{
    Mix_HaltMusic();
    Mix_PlayMusic(muzica_fundal[2], -1);
    i = 0;
    contor_asteptare = 0;
    stop = false;
    ruleaza_demo = false;
    timp = total * delta_t / 1000 + 10;
    while(in_demo)
    {
        if(!stop && !ruleaza_demo)
        {
            while(SDL_PollEvent(&eveniment))
            {
                switch(eveniment.type)
                {
                case SDL_MOUSEMOTION:
                    x_mouse = eveniment.motion.x;
                    y_mouse = eveniment.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x_mouse = eveniment.motion.x;
                    y_mouse = eveniment.motion.y;
                    if(coliziune(x_mouse, y_mouse, butoane_demo[1].locatie_buton))
                    {
                        if(delta_t - 100 > 100)
                            delta_t -= 100;
                        timp = total * delta_t / 1000 + 10;
                    }
                    else if(coliziune(x_mouse, y_mouse, butoane_demo[2].locatie_buton))
                    {
                        if(delta_t + 100 < 2000)
                            delta_t += 100;
                        timp = total * delta_t / 1000 + 10;
                    }
                    else if(coliziune(x_mouse, y_mouse, butoane_demo[3].locatie_buton))
                        stop = true;
                    break;
                }
            }
            SDL_BlitSurface(fundal_demo, NULL, buffer, NULL);
            if(alegere_b)
                SDL_BlitSurface(bpersonaj[indice_personaj], &coor_miscare[7], buffer, &locatie_budist_demo);
            else if(alegere_f)
                SDL_BlitSurface(fpersonaj[indice_personaj], &coor_miscare[7], buffer, &locatie_budist_demo);
            SDL_BlitSurface(mesaj, NULL, buffer, &locatie_mesaj);

            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[timp%10], buffer, &locatie_cifra_timp[3]);
            if(timp < 10)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_timp[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_timp[2]);
            }
            else if(timp >= 10 && timp < 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_timp[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[timp/10%10], buffer, &locatie_cifra_timp[2]);
            }
            else if(timp >= 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[timp/100], buffer, &locatie_cifra_timp[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[timp/10%10], buffer, &locatie_cifra_timp[2]);
            }

            SDL_BlitSurface(imagine_tija, NULL, buffer, &pozitie_tija);
            for(ii=1; ii<=3; ii++)
            {
                SDL_BlitSurface(butoane_demo[ii].imagine, NULL, buffer, &butoane_demo[ii].locatie_buton);
                if(coliziune(x_mouse, y_mouse, butoane_demo[ii].locatie_buton))
                    SDL_BlitSurface(disc[4], NULL, buffer, &butoane_demo[ii].locatia_discului);
                else
                    SDL_BlitSurface(disc[2], NULL, buffer, &butoane_demo[ii].locatia_discului);
            }

            varf[1] = 9-numar_discuri;
            varf[2] = 9;
            varf[3] = 9;
            for(ii = 1; ii<=3; ii++)
                for(j = 1; j<=8; j++)
                    M[ii][j] = L[ii][j] = 0;
            for(j = 9-numar_discuri; j<=8; j++)
            {
                M[1][j] = j;
                L[1][j] = j;
            }

            for(ii = 1; ii<=3; ii++)
            {
                for(j = 1; j<=8; j++)
                {
                    if(M[ii][j] != 0 && L[ii][j] != 0)
                        SDL_BlitSurface(imagine_discuri, &taiere_discuri[M[ii][j]], buffer, &locatie_discuri_tija[ii][L[ii][j]]);
                }
            }
            SDL_Flip(buffer);
        }
        for(i=1; i<=total && stop; i++)
        {
            while(SDL_PollEvent(&eveniment))
            {
                switch(eveniment.type)
                {
                case SDL_MOUSEMOTION:
                    x_mouse = eveniment.motion.x;
                    y_mouse = eveniment.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x_mouse = eveniment.motion.x;
                    y_mouse = eveniment.motion.y;
                    if(coliziune(x_mouse, y_mouse, butoane_demo[1].locatie_buton))
                    {
                        if(delta_t - 100 > 200);
                        delta_t -= 100;

                        if(delta_t == 200)
                            delta_t = delta_t;
                    }
                    else if(coliziune(x_mouse, y_mouse, butoane_demo[2].locatie_buton))
                    {
                        if(delta_t + 100 < 2000)
                            delta_t += 100;

                        if(delta_t == 2000)
                            delta_t = delta_t;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    event = false;
                    break;
                }
            }
            if(i == total)
            {
                stop = false;
                ruleaza_demo = true;
            }
            SDL_BlitSurface(fundal_demo, NULL, buffer, NULL);
            SDL_BlitSurface(imagine_tija, NULL, buffer, &pozitie_tija);
            SDL_BlitSurface(bara_demo, NULL, buffer, &locatie_bara_demo);

            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[i%10], buffer, &locatie_cifra_numar[3]);
            if(i < 10)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_numar[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_numar[2]);
            }
            else if(i >= 10 && i < 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_numar[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[i/10%10], buffer, &locatie_cifra_numar[2]);
            }
            else if(i >= 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[i/100], buffer, &locatie_cifra_numar[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[i/10%10], buffer, &locatie_cifra_numar[2]);
            }
            SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total%10], buffer, &locatie_cifra_total[3]);
            if(total < 10)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_total[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_total[2]);
            }
            else if(total >= 10 && total < 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[10], buffer, &locatie_cifra_total[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/10%10], buffer, &locatie_cifra_total[2]);
            }
            else if(total >= 100)
            {
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/100], buffer, &locatie_cifra_total[1]);
                SDL_BlitSurface(cifre[indice_personaj], &taiere_cifre[total/10%10], buffer, &locatie_cifra_total[2]);
            }
            indice_disc = M[mutari[i].sursa][varf[mutari[i].sursa]];
            M[mutari[i].sursa][varf[mutari[i].sursa]] = 0;
            L[mutari[i].sursa][varf[mutari[i].sursa]] = 0;
            varf[mutari[i].sursa]++;

            varf[mutari[i].destinatie]--;
            M[mutari[i].destinatie][varf[mutari[i].destinatie]] = indice_disc;
            L[mutari[i].destinatie][varf[mutari[i].destinatie]] = varf[mutari[i].destinatie];

            for(ii = 1; ii<=3; ii++)
            {
                for(j = 1; j<=8; j++)
                {
                    if(M[ii][j] != 0 && L[ii][j] != 0)
                        SDL_BlitSurface(imagine_discuri, &taiere_discuri[M[ii][j]], buffer, &locatie_discuri_tija[ii][L[ii][j]]);
                }
            }
            SDL_Delay(delta_t);
            SDL_Flip(buffer);
        }
        if(!stop && ruleaza_demo)
        {
            if(!contor_asteptare)
            {
                SDL_Delay(1000);
                contor_asteptare++;
            }
            while(SDL_PollEvent(&eveniment))
            {
                switch(eveniment.type)
                {
                case SDL_MOUSEMOTION:
                    x_mouse = eveniment.motion.x;
                    y_mouse = eveniment.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x_mouse = eveniment.motion.x;
                    y_mouse = eveniment.motion.y;
                    if(coliziune(x_mouse, y_mouse, butoane_demo2[3].locatie_buton))
                    {
                        in_demo = false;
                        in_joc = false;
                    }
                    if(coliziune(x_mouse, y_mouse, butoane_demo2[1].locatie_buton))
                    {
                        inca_merge = false;
                        ruleaza_meniu = true;
                        alegere = true;
                        alegere_b = false;
                        alegere_f = false;
                        indice_personaj = 0;
                        numar_discuri = 0;
                        in_demo = false;
                        contor_asteptare = 0;
                    }
                    if(coliziune(x_mouse, y_mouse, butoane_demo2[2].locatie_buton))
                    {
                        i = 0;
                        stop = false;
                        ruleaza_demo = false;
                        contor_asteptare = 0;
                    }
                    break;
                }
            }
            SDL_BlitSurface(fundal_demo, NULL, buffer, NULL);
            SDL_BlitSurface(imagine_tija, NULL, buffer, &pozitie_tija);
            for(ii = 1; ii<=3; ii++)
                SDL_BlitSurface(butoane_demo2[ii].imagine, NULL, buffer, &butoane_demo2[ii].locatie_buton);
            for(ii = 1; ii<=3; ii++)
            {
                if(coliziune(x_mouse, y_mouse, butoane_demo2[ii].locatie_buton))
                    SDL_BlitSurface(disc[4], NULL, buffer, &butoane_demo2[ii].locatia_discului);
                else
                    SDL_BlitSurface(disc[2], NULL, buffer, &butoane_demo2[ii].locatia_discului);
            }
            for(ii = 1; ii<=3; ii++)
            {
                for(j = 1; j<=8; j++)
                {
                    if(M[ii][j] != 0 && L[ii][j] != 0)
                        SDL_BlitSurface(imagine_discuri, &taiere_discuri[M[ii][j]], buffer, &locatie_discuri_tija[ii][L[ii][j]]);
                }
            }
            SDL_Flip(buffer);
        }
    }
}

void inchide()
{
    SDL_Quit();
    Mix_CloseAudio();
}

int main (int argc, char* args[])
{
    incarca_imagini();
    citeste_coordonate();
    while(in_joc)
    {
        if(ruleaza_meniu)
            meniu_principal();
        if(alegere)
            alegere_personaj();
        if(!indice_personaj)
            alegere_brahman();
        if(!numar_discuri)
            alegere_discuri();
        if(inca_merge)
            joaca();
        if(in_demo)
            mod_automat();
    }
    inchide();
    return 0;
}
