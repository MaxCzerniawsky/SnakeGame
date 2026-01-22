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
    if (temp == NULL) return;

    // Dla gracza - ¿ó³ty
    ustaw_kolor(KOLOR_ZOLTY);

    while (temp != NULL) {
        gotoxy(temp->x, temp->y);
        printf("%c", temp->znak);
        temp = temp->nastepna;
    }

    przywroc_kolor();
}

// Lub jeœli chcesz oddzieln¹ funkcjê dla przeciwnika:
void lista::wyswietl_przeciwnik() {
    waz* temp = pierwsza;
    if (temp == NULL) return;

    // Dla przeciwnika - czerwony
    ustaw_kolor(KOLOR_CZERWONY);

    while (temp != NULL) {
        gotoxy(temp->x, temp->y);
        printf("%c", temp->znak);
        temp = temp->nastepna;
    }

    przywroc_kolor();
}

void lista::czysc() {
	waz* temp = pierwsza;
	while (temp) {
		gotoxy(temp->x, temp->y); printf(" ");
		temp = temp->nastepna;
	}
}

void lista::usun_ogon() {
    if (pierwsza == NULL) return; // Lista pusta

    if (pierwsza->nastepna == NULL) {
        // Tylko g³owa - mo¿esz zakoñczyæ grê lub zostawiæ
        return;
    }

    // ZnajdŸ przedostatni element
    waz* przedostatni = pierwsza;
    while (przedostatni->nastepna != NULL && przedostatni->nastepna->nastepna != NULL) {
        przedostatni = przedostatni->nastepna;
    }

    // Wyczyœæ ogon z ekranu (ju¿ zrobione w lista_ruch)
    // Usuñ ostatni element
    delete przedostatni->nastepna;
    przedostatni->nastepna = NULL;
}

void lista_ruch(lista* waz1, int k, char ek[80][20], int* zjedz) {
    *zjedz = 0;
    if (waz1->pierwsza == NULL) return;

    waz* glowa = waz1->pierwsza;
    int nx = glowa->x;
    int ny = glowa->y;

    // Obliczanie nowej pozycji
    switch (k) {
    case 1: ny--; break;
    case 2: nx++; break;
    case 3: ny++; break;
    case 4: nx--; break;
    }

    // --- DETEKCJA KOLIZJI I ZNAKÓW ---
    char co_jest = ek[nx][ny];

    // Œmieræ: uderzenie w ramkê (#) lub cia³o wê¿a (o, s, x)
    if (co_jest == '#' || co_jest == 'o' || co_jest == 's' || co_jest == 'x') {
        *zjedz = 99;
        return;
    }

    // Zapamiêtanie ogona przed ruchem (do wyd³u¿ania lub czyszczenia)
    waz* ostatni = glowa;
    while (ostatni->nastepna != NULL) ostatni = ostatni->nastepna;
    int ost_x = ostatni->x;
    int ost_y = ostatni->y;

    // Usuniêcie ogona z mapy bitowej przed przesuniêciem
    ek[ost_x][ost_y] = ' ';

    // Przesuwanie segmentów
    waz* segment = glowa;
    int pop_x = nx;
    int pop_y = ny;
    while (segment != NULL) {
        int tx = segment->x; int ty = segment->y;
        segment->x = pop_x; segment->y = pop_y;
        pop_x = tx; pop_y = ty;
        segment = segment->nastepna;
    }

    // Wpisanie nowej pozycji g³owy do mapy ek
    ek[nx][ny] = glowa->c;

    // --- REAKCJA NA ZNAKI ---
    if (co_jest == 'R') {
        waz1->dodaj(ost_x, ost_y, 'o');
        *zjedz = 1;
    }
    else if (co_jest == 'U') {
        if (waz1->pierwsza->nastepna != NULL) waz1->usun_ogon();
        *zjedz = 2;
    }
    else if (co_jest == 'T') {
        *zjedz = 3;
    }
    else if (co_jest == 'Z') {
        *zjedz = 4;
    }
}