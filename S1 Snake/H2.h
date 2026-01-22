#ifndef H2
#define H2

#include <iostream>
#include "windows.h"
#include <conio.h>
#include <cstdlib>
#include <cmath>

// Definicje kolorów
#define KOLOR_ZOLTY (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define KOLOR_CZERWONY (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define KOLOR_NIEBIESKI (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define KOLOR_ZIELONY (FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define KOLOR_BIALY (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

struct waz {
    int x, y;
    char c;
    waz* nastepna;
    waz();
    char znak;
};

struct lista {
    waz* pierwsza;
    lista();
    void dodaj(int x, int y, char c);
    void wyswietl();
    void czysc();
    void usun_ogon();
    void ruch(int k, char ek[80][20]);
    void wyswietl_przeciwnik();
};

void image();

void ustaw_kolor(WORD kolor);
void przywroc_kolor();

int sprawdz_stan_gry(int punkty, int zycia);

int sprawdz_kolizje(lista* waz1);

void kla(int* kier, int* koniec);

void lista_ruch(lista* waz1, int k, char ek[80][20], int* zjedz);

void waz_dodaj(lista* l, int x, int y, char c);


void gotoxy(int x, int y);
void jedzenie(char ek[80][20], char znak);
void druk_e(char ek[80][20]);
void ini(char ek[80][20]);
void clean();
void ramka(char ek[80][20]);
int oblicz_kierunek_ai(lista* waz2, char ek[80][20], int aktualny_kier2, int* licznik_odskoku, int aktualne_zycia, int poziom);

#endif