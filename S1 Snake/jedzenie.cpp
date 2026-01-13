#include "H2.h"

void jedzenie(char ek[80][20]) {
    int x, y;
    x = rand() % 55 + 2;
    y = rand() % 17 + 2;
    ek[x][y] = 'R';
    gotoxy(x, y); 
    printf("R");
}
void ini(char ek[80][20]) {

    int x, y;
    //for (int i = 1; i< 20; i++)
    {
        x = rand() % 55 + 2;
        y = rand() % 17 + 2;
        ek[x][y] = 'R';
    }
}
void druk_e(char ek[80][20]) {
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 80; j++) {
            gotoxy(j, i); printf("%c", ek[j][i]);
        }
}