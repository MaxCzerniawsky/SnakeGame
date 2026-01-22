#include "H2.h"

void jedzenie(char ek[80][20], char znak) {
    int x, y;
    do {
        x = rand() % 78 + 1;
        y = rand() % 18 + 1;
    } while (ek[x][y] != ' ');

    if (znak == 'R') ustaw_kolor(KOLOR_NIEBIESKI);
    else if (znak == 'U') ustaw_kolor(KOLOR_ZIELONY);
    else if (znak == 'T') ustaw_kolor(KOLOR_CZERWONY); // Trucizna - czerwona
    else if (znak == 'Z') ustaw_kolor(KOLOR_BIALY); // ¯ycie - fioletowe

    ek[x][y] = znak;
    gotoxy(x, y);
    printf("%c", znak);
    przywroc_kolor();
}


// Dodaj tê funkcjê:
int sprawdz_kolizje(lista* waz) {
    if (waz->pierwsza == NULL) return 0;

    // Pobieramy pozycjê g³owy
    int x = waz->pierwsza->x;
    int y = waz->pierwsza->y;

    // 1. Kolizja z ramk¹ (#)
    if (x <= 0 || x >= 79 || y <= 0 || y >= 19) {
        return 1;
    }

    // Uwaga: Kolizje z cia³ami wê¿y (znaki o, x, s, =) 
    // s¹ teraz wy³apywane bezpoœrednio w funkcji lista_ruch, 
    // co jest bezpieczniejsze w tym silniku gry.
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