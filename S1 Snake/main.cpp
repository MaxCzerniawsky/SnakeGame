#include "H2.h"
#include <direct.h>

void image()
{
    // Usuń zbędne gotoxy - przeszkadzają w wyświetlaniu obrazka
    system("cls");  // Wyczyść ekran przed wyświetleniem obrazka

    FILE* plik;
    errno_t err;
    char nazwa_obrazka[] = "..\\image1.txt";

    err = fopen_s(&plik, nazwa_obrazka, "r");

    if (err != 0 || plik == NULL)
    {
        printf("Nie mozna otworzyc pliku: %s\n", nazwa_obrazka);
        printf("Kod bledu: %d\n", err);

        // Sprawdź, czy plik istnieje w bieżącym katalogu
        printf("Upewnij sie, ze plik znajduje sie w katalogu:\n");
        system("cd");  // Pokazuje aktualny katalog

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

void ramka() {
    // Możesz dodać kolor dla ramki
    ustaw_kolor(KOLOR_BIALY); // Różowa ramka

    for (int i = 0; i < 80; i++) {
        gotoxy(i, 0);
        printf("#");
        gotoxy(i, 19);
        printf("#");
    }

    for (int i = 0; i < 20; i++) {
        gotoxy(0, i);
        printf("#");
        gotoxy(79, i);
        printf("#");
    }

    przywroc_kolor();
}


void menu(const char* imieGracza) {

    clean();

	gotoxy(3, 5);
	printf("*");
	gotoxy(5, 5);
	printf("%s", imieGracza);

	gotoxy(15, 5);
	printf("* \n");
	printf("\n");
	printf("    1 - Play \n");
	printf("    2 - Name \n");
	printf("    3 - Raport \n");
	printf("    4 - Reset \n");
	printf("    5 - EXIT \n");
	//printf("    6 - Read data \n");
	printf("\n");
	gotoxy(0, 0);
	ramka();
	gotoxy(4, 15);
	printf("    Press number to choose option");

}


int game() {
    int punkty = 0;
    int punkty_ai = 0;
    int zycia = 3;
    int poziom = 1;
    char ek[80][20] = { ' ' };
    int kier = 1, koniec = 0, kier2 = 1;
    int licznik_odskoku_ai = 0;

    lista* waz1 = NULL, * waz2 = NULL;

    auto resetuj_weze = [&]() {
        if (waz1) delete waz1;
        if (waz2) delete waz2;
        waz1 = new lista();
        waz2 = new lista();
        waz1->dodaj(10, 10, 'o'); waz1->dodaj(11, 10, 'o'); waz1->dodaj(12, 10, 'x'); // Gracz
        waz2->dodaj(40, 5, 's'); waz2->dodaj(41, 5, 's'); waz2->dodaj(42, 5, 's');    // AI
        };

    resetuj_weze();
    ini(ek);
    ramka(); // Zakładam, że ramka wypełnia ek znakami '#'
    druk_e(ek);

    jedzenie(ek, 'R');
    jedzenie(ek, 'U');

    while (!koniec) {
        // Interfejs
        gotoxy(2, 25);
        printf("POZIOM: %d | TWOJE PUNKTY: %d | ZYCIA: %d      ", poziom, punkty, zycia);
        gotoxy(2, 26);
        printf("PUNKTY PRZECIWNIKA: %d / 10      ", punkty_ai);

        kla(&kier, &koniec);
        if (koniec) break;

        kier2 = oblicz_kierunek_ai(waz2, ek, kier2, &licznik_odskoku_ai);

        int wynik_gracz = 0;
        int wynik_ai = 0;

        lista_ruch(waz1, kier, ek, &wynik_gracz);
        lista_ruch(waz2, kier2, ek, &wynik_ai);

        // --- OBSŁUGA GRACZA ---
        if (wynik_gracz == 99) {
            system("cls"); printf("\n\n PRZEGRANA! Kolizja!");
            break;
        }
        if (wynik_gracz == 1) { punkty++; jedzenie(ek, 'R'); }
        else if (wynik_gracz == 2) { punkty--; jedzenie(ek, 'U'); }
        else if (wynik_gracz == 3) { zycia--; jedzenie(ek, 'T'); }
        else if (wynik_gracz == 4) { zycia++; jedzenie(ek, 'Z'); }

        // --- OBSŁUGA AI ---
        if (wynik_ai == 99) {
            system("cls"); printf("\n\n ZWYCIESTWO! Przeciwnik sie rozbil!");
            break;
        }
        if (wynik_ai != 0) {
            licznik_odskoku_ai = 4;
            if (wynik_ai == 1) { punkty_ai++; jedzenie(ek, 'R'); }
            else if (wynik_ai == 2) jedzenie(ek, 'U');
            else if (wynik_ai == 3) jedzenie(ek, 'T');
            else if (wynik_ai == 4) jedzenie(ek, 'Z');
        }

        // --- PROGRESJA POZIOMU ---
        if (poziom == 1 && punkty >= 10) {
            poziom = 2; punkty = 0; punkty_ai = 0;
            system("cls"); printf("\n\n AWANS NA POZIOM 2!"); Sleep(1500);
            ini(ek); ramka(); druk_e(ek);
            resetuj_weze();
            jedzenie(ek, 'R'); jedzenie(ek, 'U'); jedzenie(ek, 'T'); jedzenie(ek, 'Z');
            continue;
        }

        // --- KONIEC GRY ---
        if (zycia <= 0) {
            system("cls"); printf("\n\n PRZEGRANA! Straciles zycia.");
            break;
        }
        if (punkty_ai >= 10) {
            system("cls"); printf("\n\n PRZEGRANA! AI zebralo 10 punktow.");
            break;
        }

        // Rysowanie
        ustaw_kolor(KOLOR_ZOLTY); waz1->wyswietl();
        ustaw_kolor(KOLOR_CZERWONY); waz2->wyswietl();
        przywroc_kolor();

        Sleep(250);
        waz1->czysc(); waz2->czysc();
    }

    delete waz1; delete waz2;
    return punkty;
}

void waz_dodaj(lista* l, int x, int y, char c) {
    l->dodaj(x, y, c);
}

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
