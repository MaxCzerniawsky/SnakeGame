#include "H2.h"
lista::lista() {
	pierwsza = 0;
}

void lista::dodaj(int x, int y, char c) {
	waz* nowa = new waz;

	nowa->x = x;
	nowa->y = y;
	nowa->c = c;

	if (pierwsza == 0) {
		nowa->c = 'E';
		pierwsza = nowa;
	}
	else
	{
		waz* temp = pierwsza;
		while (temp->nastepna) {
			temp = temp->nastepna;
		}
		temp->nastepna = nowa;
		nowa->nastepna = 0;
	}
}

void lista::wyswietl() {
	waz* temp = pierwsza;
	while (temp) {
		gotoxy(temp->x, temp->y);
		printf("%c", temp->c);
		temp = temp->nastepna;
	}
}

void lista::czysc() {
	waz* temp = pierwsza;
	while (temp) {
		gotoxy(temp->x, temp->y); printf(" ");
		temp = temp->nastepna;
	}
}

void lista_ruch(lista* waz1, int k, char ek[80][20], int* zjedz) {
    *zjedz = 0;
    if (waz1->pierwsza == NULL) return;

    waz* temp = waz1->pierwsza;
    int x1 = temp->x;
    int y1 = temp->y;
    int x2 = x1; // Inicjalizacja na wypadek gdyby waz mial tylko g³owê
    int y2 = y1;

    // Ruch g³owy
    switch (k) {
    case 1: temp->y -= 1; break;
    case 2: temp->x += 1; break;
    case 3: temp->y += 1; break;
    case 4: temp->x -= 1; break;
    }

    // Sprawdzenie jedzenia
    int kz = 0;
    if (ek[temp->x][temp->y] == 'R') {
        kz = 1;
        ek[temp->x][temp->y] = ' ';
    }

    // Przesuwanie reszty cia³a
    waz* segment = temp->nastepna;
    while (segment) {
        int tx = segment->x;
        int ty = segment->y;
        segment->x = x1;
        segment->y = y1;
        x1 = tx;
        y1 = ty;
        x2 = x1; y2 = y1; // Zapamiêtujemy ostatni segment
        segment = segment->nastepna;
    }

    if (kz == 1) {
        // x2, y2 to teraz na pewno pozycja ostatniego elementu przed ruchem
        waz1->dodaj(x2, y2, 'o');
        jedzenie(ek);
        *zjedz = 1;
    }
}
