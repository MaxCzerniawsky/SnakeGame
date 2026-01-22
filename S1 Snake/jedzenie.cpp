#include "H2.h"

void jedzenie(char ek[80][20], char znak) {
    int x, y;
    do {
        x = rand() % 78 + 1;
        y = rand() % 18 + 1;
    } while (ek[x][y] != ' ');

    if (znak == 'R') ustaw_kolor(KOLOR_NIEBIESKI); // Racje ¿ywnoœciowe
    else if (znak == 'U') ustaw_kolor(KOLOR_ZIELONY); // Udrêka
    else if (znak == 'T') ustaw_kolor(KOLOR_CZERWONY); // Trucizna - czerwona
    else if (znak == 'Z') ustaw_kolor(KOLOR_BIALY); // ¯ycie - fioletowe

    ek[x][y] = znak;
    gotoxy(x, y);
    printf("%c", znak);
    przywroc_kolor();
}

void aktualizuj_pulapki(char ek[80][20], int* px, int* py, clock_t* ostatnia_zmiana, int* stan_pulapki) {
    clock_t teraz = clock();
    double minelo = (double)(teraz - *ostatnia_zmiana) / CLOCKS_PER_SEC;

    // Stan 1: Przeszkoda jest widoczna (trwa 3 sekundy)
    if (*stan_pulapki == 1 && minelo >= 3.0) {
        if (*px != -1) {
            ek[*px][*py] = ' '; 
            gotoxy(*px, *py); printf(" ");
        }
        *stan_pulapki = 0; 
        *ostatnia_zmiana = teraz;
    }
    
    else if (*stan_pulapki == 0 && minelo >= 2.0) {
        
        int nx, ny;
        do {
            nx = rand() % 78 + 1;
            ny = rand() % 18 + 1;
        } while (ek[nx][ny] != ' ');

        *px = nx; *py = ny;
        ek[*px][*py] = '#'; 

        ustaw_kolor(KOLOR_CZERWONY);
        gotoxy(*px, *py); printf("#");
        przywroc_kolor();

        *stan_pulapki = 1; 
        *ostatnia_zmiana = teraz;
    }
}


int sprawdz_kolizje(lista* waz) {
    if (waz->pierwsza == NULL) return 0;

    // Pobieramy pozycjê g³owy
    int x = waz->pierwsza->x;
    int y = waz->pierwsza->y;

    // 1. Kolizja z ramk¹ (#)
    if (x <= 0 || x >= 79 || y <= 0 || y >= 19) {
        return 1;
    }

    
    return 0;
}

void ini(char ek[80][20]) {
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 20; j++) {
            ek[i][j] = ' ';
        }
    }
}

void druk_e(char ek[80][20]) {
    for (int j = 0; j < 20; j++) {
        for (int i = 0; i < 80; i++) {
            if (ek[i][j] != ' ') { // Rysuj tylko niepuste pola
                gotoxy(i, j);
                printf("%c", ek[i][j]);
            }
        }
    }
}