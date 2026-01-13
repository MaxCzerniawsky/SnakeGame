#include "H2.h"

void clean() {
    for (int i = 25; i >= 0; i--) {
        gotoxy(0, i);
        printf("                                                                                     ");
        Sleep(50);
    }
}

void ramka() {
	char aa;
	aa = 218; gotoxy(1, 1); printf("%c", aa);
	aa = 191; gotoxy(80, 1); printf("%c", aa);
	aa = 192; gotoxy(1, 24); printf("%c", aa);
	aa = 217; gotoxy(80, 24); printf("%c", aa);
	aa = 196;
	for (int i = 2; i < 80; i++) {
		gotoxy(i, 1); printf("%c", aa);
		gotoxy(i, 24); printf("%c", aa);
	}
	aa = 179;
	for (int i = 2; i < 24; i++) {
		gotoxy(1, i); printf("%c", aa);
		gotoxy(80, i); printf("%c", aa);
	}
	gotoxy(2, 25);
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
    char ek[80][20] = { ' ' };
    int kier = 1, koniec = 0;

    lista* waz1 = new lista(); // Używamy new, skoro to C++

    ramka();
    ini(ek);
    druk_e(ek);

    // Inicjalizacja węża
    waz1->dodaj(10, 10, 'o');
    waz1->dodaj(11, 10, 'o');
    waz1->dodaj(12, 10, 'o');
    waz1->dodaj(13, 10, 'x');

    gotoxy(2, 25);
    printf("Punkty: %d", punkty);

    while (!koniec) {
        kla(&kier, &koniec); // Pamiętaj: w H2.h i w definicji kla musi być int* koniec
        if (koniec) break;

        int czy_zjadl = 0;

        // Wywołujemy funkcję ruchu, która zwróci punkt przez &czy_zjadl
        lista_ruch(waz1, kier, ek, &czy_zjadl);

        if (czy_zjadl == 1) {
            punkty++;
            gotoxy(2, 25);
            printf("Punkty: %d  ", punkty);
        }

        waz1->wyswietl();
        Sleep(300);
        waz1->czysc();
    }

    delete waz1;
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

        case '4':
            r = 0;
            break;
        }
    }

    return 0;
}
