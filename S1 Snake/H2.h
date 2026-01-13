#ifndef H2
#define H2

#include <iostream>
#include "windows.h"
#include <conio.h>
#include <cstdlib>

struct waz {
    int x, y;
    char c;
    waz* nastepna;
    waz();
};

struct lista {
    waz* pierwsza;
    lista();
    void dodaj(int x, int y, char c);
    void wyswietl();
    void czysc();
    void ruch(int k, char ek[80][20]);
};


void kla(int* kier, int* koniec);

void lista_ruch(lista* waz1, int k, char ek[80][20], int* zjedz);

void waz_dodaj(lista* l, int x, int y, char c);


void gotoxy(int x, int y);
void jedzenie(char ek[80][20]);
void druk_e(char ek[80][20]);
void ini(char ek[80][20]);
void clean();
void ramka();

#endif