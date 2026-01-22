#include "H2.h"

waz::waz() {
	nastepna = 0;
	x = 1; 
	y = 1, 
	c = 'o';
}

int oblicz_kierunek_ai(lista* waz2, char ek[80][20], int aktualny_kier2, int* licznik_odskoku, int aktualne_zycia, int poziom) {
    if (waz2->pierwsza == NULL) return 2;

    waz* glowa = waz2->pierwsza;
    int x = glowa->x;
    int y = glowa->y;

    int cel_x = -1, cel_y = -1;
    float zasieg_standard = 15.0f; // Dla R i Z
    float zasieg_robak = 5.0f;     // Dla robaczka O

    float d_najblizszy_R = 999.0f, r_x = -1, r_y = -1;
    float d_najblizszy_Z = 999.0f, z_x = -1, z_y = -1;
    float d_najblizszy_O = 999.0f, o_x = -1, o_y = -1;

    // 1. SKANOWANIE OTOCZENIA
    for (int i = 1; i < 79; i++) {
        for (int j = 1; j < 19; j++) {
            char znak = ek[i][j];
            float d = sqrt(pow(x - i, 2) + pow(y - j, 2));

            // Skanowanie dla standardowych literek (zasieg 15)
            if (d <= zasieg_standard) {
                if (znak == 'R' && d < d_najblizszy_R) { d_najblizszy_R = d; r_x = i; r_y = j; }
                if (znak == 'Z' && d < d_najblizszy_Z) { d_najblizszy_Z = d; z_x = i; z_y = j; }
            }

            // Skanowanie dla robaczka (zasieg tylko 5)
            if (znak == 'O' && d <= zasieg_robak) {
                if (d < d_najblizszy_O) { d_najblizszy_O = d; o_x = i; o_y = j; }
            }
        }
    }

    // 2. WYBÓR CELU ZGODNIE Z TWOIMI PRIORYTETAMI
    if (poziom == 1) {
        if (r_x != -1) { cel_x = r_x; cel_y = r_y; }
        else if (z_x != -1) { cel_x = z_x; cel_y = z_y; }
    }
    else if (poziom == 2) {
        if (aktualne_zycia <= 1) { // Kryzys: Z -> R
            if (z_x != -1) { cel_x = z_x; cel_y = z_y; }
            else if (r_x != -1) { cel_x = r_x; cel_y = r_y; }
        }
        else { // Normalnie: R -> Z
            if (r_x != -1) { cel_x = r_x; cel_y = r_y; }
            else if (z_x != -1) { cel_x = z_x; cel_y = z_y; }
        }
    }
    else if (poziom == 3) {
        if (aktualne_zycia <= 1) { // Kryzys na P3: Z -> R -> O
            if (z_x != -1) { cel_x = z_x; cel_y = z_y; }
            else if (r_x != -1) { cel_x = r_x; cel_y = r_y; }
            else if (o_x != -1) { cel_x = o_x; cel_y = o_y; }
        }
        else { // Normalnie na P3: O -> R -> Z
            if (o_x != -1) { cel_x = o_x; cel_y = o_y; } // Robak ma priorytet, ale tylko w zasiegu 5!
            else if (r_x != -1) { cel_x = r_x; cel_y = r_y; }
            else if (z_x != -1) { cel_x = z_x; cel_y = z_y; }
        }
    }

    // 3. WYZNACZANIE KIERUNKU I BEZPIECZEÑSTWO (bez zmian)
    int najlepszy_kierunek = aktualny_kier2;
    if (cel_x != -1) {
        if (abs(x - cel_x) > abs(y - cel_y)) najlepszy_kierunek = (x < cel_x) ? 2 : 4;
        else najlepszy_kierunek = (y < cel_y) ? 3 : 1;
    }

    int sprawdzany_kier = najlepszy_kierunek;
    for (int i = 0; i < 4; i++) {
        int nx = x, ny = y;
        if (sprawdzany_kier == 1) ny--;
        else if (sprawdzany_kier == 2) nx++;
        else if (sprawdzany_kier == 3) ny++;
        else if (sprawdzany_kier == 4) nx--;

        if (nx > 0 && nx < 79 && ny > 0 && ny < 19) {
            char pole = ek[nx][ny];
            if (pole != '#' && pole != 'o' && pole != 's' && pole != 'x' && pole != 'U' && pole != 'T') {
                return sprawdzany_kier;
            }
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