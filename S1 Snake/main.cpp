#include "H2.h"
#include <direct.h>

void image()
{
    
    system("cls");  

    FILE* plik;
    errno_t err;
    char nazwa_obrazka[] = "..\\image1.txt";

    err = fopen_s(&plik, nazwa_obrazka, "r");

    if (err != 0 || plik == NULL)
    {
        printf("Nie mozna otworzyc pliku: %s\n", nazwa_obrazka);
        printf("Kod bledu: %d\n", err);

        
        printf("Upewnij sie, ze plik znajduje sie w katalogu:\n");
        system("cd");  

        return;
    }

    int znak;
    while ((znak = fgetc(plik)) != EOF)
    {
        putchar(znak);
    }

    fclose(plik);
    Sleep(3000);
}

void clean() {
    for (int i = 25; i >= 0; i--) {
        gotoxy(0, i);
        printf("                                                                                     ");
        Sleep(50);
    }
}

int sprawdz_stan_gry(int punkty, int zycia) {
    if (punkty >= 10) {
        system("cls");
        printf("GRATULACJE! Wygrales! Zdobyte punkty: %d\n", punkty);
        return 1; // Koniec gry - wygrana
    }
    if (zycia <= 0) {
        system("cls");
        printf("GAME OVER! Straciles wszystkie zycia. Punkty: %d\n", punkty);
        return 1; // Koniec gry - przegrana
    }
    return 0; // Graj dalej
}

void ramka(char ek[80][20]) {
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 79 || j == 0 || j == 19) {
                ek[i][j] = '#'; // WPISANIE DO PAMIĘCI GRY
                gotoxy(i, j);
                printf("#");    // WYŚWIETLENIE NA EKRANIE
            }
        }
    }
}


void menu(const char* imieGracza) {
    char ek[80][20]; 
    clean();

    
    ramka(ek);

    gotoxy(5, 5);
    printf("GRACZ: %s", imieGracza);

    gotoxy(5, 7);
    printf("1 - Play");
    gotoxy(5, 8);
    printf("2 - Name");
    gotoxy(5, 9);
    printf("3 - Raport");
    gotoxy(5, 10);
    printf("4 - Reset");
    gotoxy(5, 11);
    printf("5 - EXIT");

    gotoxy(4, 15);
    printf("Press number to choose option");
}


//GAME LOGIC

void aktualizuj_robaczka(int* rx, int* ry, int* rkier, int* rkroki, char ek[80][20]) {
    if (*rx == -1) return;

    ek[*rx][*ry] = ' '; // Czyścimy starą pozycję w tablicy
    gotoxy(*rx, *ry); printf(" ");

    if (*rkroki <= 0) {
        *rkier = rand() % 4 + 1;
        *rkroki = rand() % 3 + 3;
    }

    int nx = *rx, ny = *ry;
    if (*rkier == 1) ny--;
    else if (*rkier == 2) nx++;
    else if (*rkier == 3) ny++;
    else if (*rkier == 4) nx--;

    if (nx <= 0 || nx >= 79 || ny <= 0 || ny >= 19 || ek[nx][ny] != ' ') {
        *rkier = (*rkier % 4) + 1;
        *rkroki = 0;
    }
    else {
        *rx = nx; *ry = ny;
        (*rkroki)--;
    }

    ek[*rx][*ry] = 'O';
    ustaw_kolor(KOLOR_ZIELONY);
    gotoxy(*rx, *ry); printf("O");
    przywroc_kolor();
}

void przygotuj_poziom(int nr, char ek[80][20], int* px, int* py) {
    system("cls");
    printf("\n\n   AWANS NA POZIOM %d!", nr);
    Sleep(1500);
    system("cls");
    clean();
    ini(ek);
    ramka(ek);
    druk_e(ek);

    
    jedzenie(ek, 'R'); // Racje żywnościowe
    jedzenie(ek, 'U'); // Utrata jedzenia

    
    if (nr >= 2) {
        jedzenie(ek, 'T'); // Trucizna (utrata zdrowia)
        jedzenie(ek, 'Z'); // Życie
    }

    // Robaczek tylko na Poziomie 3
    if (nr == 3) {
        *px = 40;
        *py = 10;
    }
}

int game() {
    int pulapka_x = -1, pulapka_y = -1;
    int stan_pulapki = 0; 
    clock_t czas_pulapki = clock();
    int punkty = 0, punkty_ai = 0, zycia = 3, poziom = 1;
    char ek[80][20] = { ' ' };
    int kier = 1, koniec = 0, kier2 = 1, licznik_odskoku_ai = 0;
    int robak_x = -1, robak_y = -1, robak_kier = 0, robak_kroki = 0;

    lista* waz1 = new lista(), * waz2 = new lista();
    auto resetuj_weze = [&]() {
        waz1->wyczysc_liste(); waz2->wyczysc_liste(); 
        waz1->dodaj(10, 10, 'o'); waz1->dodaj(11, 10, 'o'); waz1->dodaj(12, 10, 'x');
        waz2->dodaj(40, 5, 's'); waz2->dodaj(41, 5, 's'); waz2->dodaj(42, 5, 's');
        };

    //GENERACJA POZIOMU 1

    resetuj_weze();
    ini(ek); 
    ramka(ek); 
    druk_e(ek);
    jedzenie(ek, 'R');
    jedzenie(ek, 'U');

    while (!koniec) {
        // UI
        gotoxy(2, 25); printf("POZIOM: %d | TWOJE PUNKTY: %d | ZYCIA: %d      ", poziom, punkty, zycia);
        gotoxy(2, 26); printf("PUNKTY PRZECIWNIKA: %d / 10      ", punkty_ai);

        // Sterowanie
        kla(&kier, &koniec);
        kier2 = oblicz_kierunek_ai(waz2, ek, kier2, &licznik_odskoku_ai, zycia, poziom);

        // Robaczek
        if (poziom == 3) aktualizuj_robaczka(&robak_x, &robak_y, &robak_kier, &robak_kroki, ek);

        // Ruch
        int w_g = 0, w_ai = 0;
        lista_ruch(waz1, kier, ek, &w_g);
        lista_ruch(waz2, kier2, ek, &w_ai);

        // --- LOGIKA PUNKTACJI GRACZA ---
        if (w_g == 99) { zycia = 0; system("cls"); printf("\n\n KRAKSA!"); Sleep(1000); }
        if (w_g == 1) { punkty++; jedzenie(ek, 'R'); }
        if (w_g == 3) { zycia--; jedzenie(ek, 'T'); }
        if (w_g == 4) { zycia++; jedzenie(ek, 'Z'); }
        if (w_g == 5) { punkty += 10; robak_x = -1; }

        // --- LOGIKA PUNKTACJI AI ---
        if (w_ai == 99) { system("cls"); printf("\n\n Martin się rozbił!"); break; }
        if (w_ai == 1) { punkty_ai++; jedzenie(ek, 'R'); }
        if (w_ai == 5) { punkty_ai += 10; robak_x = -1; }

        // --- PROGRESJA ---
        if (poziom == 1 && punkty >= 2) {
            poziom = 2; punkty = 0; punkty_ai = 0;
            przygotuj_poziom(2, ek, &robak_x, &robak_y);
            resetuj_weze(); continue;
        }
        if (poziom == 2 && punkty >= 2) {
            poziom = 3; punkty = 0; punkty_ai = 0;
            przygotuj_poziom(3, ek, &robak_x, &robak_y);
            resetuj_weze(); continue;
        }
        if (poziom == 3) {
            aktualizuj_pulapki(ek, &pulapka_x, &pulapka_y, &czas_pulapki, &stan_pulapki);
        }
        // --- WARUNEK ZWYCIĘSTWA (Koniec Poziomu 3) ---
        if (poziom == 3 && punkty >= 2) {
            system("cls");
            ustaw_kolor(KOLOR_ZIELONY);

            // Wyświetlanie napisu na środku ekranu
            gotoxy(30, 10);
            printf("==========================");
            gotoxy(34, 11);
            printf("DAVE ZWYCIEZYL!");
            gotoxy(30, 12);
            printf("==========================");

            Sleep(3000); // Wyświetlaj przez 3 sekundy
            przywroc_kolor();
            koniec = 1;  // Przerwij pętlę i zakończ grę
            continue;
        }

        // Koniec gry
        if (zycia <= 0 || punkty_ai >= 10 || sprawdz_stan_gry(punkty, zycia)) break;

        // Rysowanie
        ustaw_kolor(KOLOR_ZOLTY);
        waz1->wyswietl(); 

        ustaw_kolor(KOLOR_NIEBIESKI); 
        waz2->wyswietl(); 

        przywroc_kolor();

        Sleep(250);
        waz1->czysc(); waz2->czysc();
    }

    delete waz1; delete waz2;
    return punkty;
}

// wąż
void waz_dodaj(lista* l, int x, int y, char c) {
    l->dodaj(x, y, c);
}


//MAIN
int main() {

    struct dd {
        int p;
        char t[20];
    };

    char playerName[20] = "SnakeName";
    char choice;
    int r = 1;

    image();
    _getch(); //zatrzymanie na chwile

    while (r == 1) {
        system("CLS");
        menu(playerName);

        scanf_s(" %c", &choice, 1);

        switch (choice) {

        case '1': {
            int zdobyte_punkty;

            system("CLS");
            clean();

            zdobyte_punkty = game();   // ODBIÓR PUNKTÓW Z GRY

            FILE* f = NULL;
            if (fopen_s(&f, "data.dat", "a") == 0 && f != NULL) {
                fprintf(f, "%d %s\n", zdobyte_punkty, playerName);
                fclose(f);
            }

            break;
        }

        case '2':
            system("CLS");
            clean();
            gotoxy(5, 6);
            printf("Your name (maximum of 19 characters) -> ");
            scanf_s("%19s", playerName, (unsigned)sizeof(playerName));
            break;

        case '3': {
            system("CLS");
            clean();

            FILE* local_stream = NULL;
            errno_t local_err;

            local_err = fopen_s(&local_stream, "data.dat", "r");
            if (local_err != 0 || local_stream == NULL) {
                printf("ERROR: file couldn't be read\n");
                printf("Press any key...");
                while (!_kbhit()) {}
                break;
            }

            printf("=== Score table ===\n\n");

            struct dd tabela[100];
            int ilosc = 0;

            while (ilosc < 100 &&
                fscanf_s(local_stream, "%d %19s",
                    &tabela[ilosc].p,
                    tabela[ilosc].t,
                    (unsigned)sizeof(tabela[ilosc].t)) == 2)
            {
                ilosc++;
            }

            fclose(local_stream);

            for (int i = 0; i < ilosc - 1; i++) {
                for (int j = 0; j < ilosc - i - 1; j++) {
                    if (tabela[j].p < tabela[j + 1].p) {
                        struct dd temp = tabela[j];
                        tabela[j] = tabela[j + 1];
                        tabela[j + 1] = temp;
                    }
                }
            }

            if (ilosc == 0) {
                printf("No scores available.\n");
            }
            else {
                for (int i = 0; i < ilosc; i++) {
                    printf("%d. %d - %s\n", i + 1, tabela[i].p, tabela[i].t);
                }
            }

            printf("\nPress any key...");
            while (!_kbhit()) {}
            break;
        }

        case '4':{
            FILE * local_stream = NULL;
            errno_t local_err;


            local_err = fopen_s(&local_stream, "data.dat", "w");

            if (local_err == 0 && local_stream != NULL) {
                fclose(local_stream);
                printf("Data has been reset\n");
            }
            else {
                printf("ERROR: file couldn't be read\n");
            }

            printf("\nPress any button...");
            while (!_kbhit()) {}
            system("CLS");
            break;
        }
        case '5':
            r = 0;
            break;
        }
    }

    return 0;
}
