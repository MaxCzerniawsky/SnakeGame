#include "H2.h"

waz::waz() {
	nastepna = 0;
	x = 1; 
	y = 1, 
	c = 'o';
}

int oblicz_kierunek_ai(lista* waz2, char ek[80][20], int aktualny_kier2, int* licznik_odskoku) {
    if (waz2->pierwsza == NULL) return 2;

    waz* glowa = waz2->pierwsza;
    int x = glowa->x;
    int y = glowa->y;

    int cel_x = -1, cel_y = -1;
    int zagrozenie_x = -1, zagrozenie_y = -1;
    float min_dystans_celu = 16.0; // Zasiêg wzroku ograniczony do 15

    // 1. OBS£UGA ODSKOKU (Ucieczka od œciany po zjedzeniu)
    if (*licznik_odskoku > 0) {
        (*licznik_odskoku)--;
        if (x < 10) return 2;
        if (x > 70) return 4;
        if (y < 5) return 3;
        if (y > 15) return 1;
    }

    // 2. SKANOWANIE OTOCZENIA (Tylko w zasiêgu wzroku)
    for (int i = 1; i < 79; i++) {
        for (int j = 1; j < 19; j++) {
            char znak = ek[i][j];
            float d = sqrt(pow(x - i, 2) + pow(y - j, 2));

            // Widzi tylko to, co jest blisko (d < 15)
            if (d < 15.0) {
                // Szukamy celu (R lub Z)
                if (znak == 'R' || znak == 'Z') {
                    if (d < min_dystans_celu) {
                        min_dystans_celu = d;
                        cel_x = i; cel_y = j;
                    }
                }
                // Wykrywamy zagro¿enie blisko (U lub T poni¿ej 4 kratek)
                if ((znak == 'U' || znak == 'T') && d < 4.0) {
                    zagrozenie_x = i; zagrozenie_y = j;
                }
            }
        }
    }

    // 3. DECYZJA O KIERUNKU
    int najlepszy_kierunek = aktualny_kier2;

    if (zagrozenie_x != -1) {
        // UCIECZKA: Jeœli zagro¿enie jest blisko, skrêæ pod k¹tem 90 stopni
        najlepszy_kierunek = (aktualny_kier2 % 4) + 1;
    }
    else if (cel_x != -1) {
        // POGOÑ: IdŸ w stronê celu
        if (abs(x - cel_x) > abs(y - cel_y)) {
            najlepszy_kierunek = (x < cel_x) ? 2 : 4;
        }
        else {
            najlepszy_kierunek = (y < cel_y) ? 3 : 1;
        }
    }
    else {
        // B£¥KANIE: Jeœli nic nie widzi, unikaj tylko œcian (margines 3 pola)
        if (x <= 3 || x >= 76 || y <= 3 || y >= 16) {
            najlepszy_kierunek = (aktualny_kier2 % 4) + 1;
        }
    }

    // 4. WERYFIKACJA BEZPIECZEÑSTWA (Nie wchodŸ w œciany i ogony)
    int sprawdzany_kier = najlepszy_kierunek;
    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;
        if (sprawdzany_kier == 1) ny--;
        else if (sprawdzany_kier == 2) nx++;
        else if (sprawdzany_kier == 3) ny++;
        else if (sprawdzany_kier == 4) nx--;

        char przyszle_pole = ek[nx][ny];
        // AI nie wejdzie na pole, jeœli jest tam œciana, cia³o lub trucizna
        if (nx > 0 && nx < 79 && ny > 0 && ny < 19 &&
            przyszle_pole != '#' && przyszle_pole != 'o' && przyszle_pole != 's' &&
            przyszle_pole != 'x' && przyszle_pole != 'U' && przyszle_pole != 'T') {
            return sprawdzany_kier;
        }
        sprawdzany_kier = (sprawdzany_kier % 4) + 1;
    }

    return aktualny_kier2;
}

void ustaw_kolor(WORD kolor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, kolor);
}

void przywroc_kolor() {
    ustaw_kolor(KOLOR_BIALY);
}