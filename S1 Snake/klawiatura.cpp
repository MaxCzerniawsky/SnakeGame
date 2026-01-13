#include "H2.h"

void kla(int* kier, int* koniec) { // Zmienione z bool* na int*
    if (_kbhit()) {
        unsigned char znak = _getch();
        switch (znak) {
        case 224: // klawisze specjalne
            znak = _getch();
            switch (znak) {
            case 72: // strza³ka w górê 
                if (*kier != 3) {
                    *kier = 1;
                }
                break;
            case 80: // strza³ka w dó³ 
                if (*kier != 1) {
                    *kier = 3;
                }
                break;
            case 75: // strza³ka w lewo
                if (*kier != 2) {
                    *kier = 4;
                }
                break;
            case 77: // strza³ka w prawo
                if (*kier != 4) {
                    *kier = 2;
                }
                break;
            }
            znak = 0;
            break;
        case 13: // ENTER
            break;
        case 27: // ESC
            *koniec = 1; // Zmienione z true na 1
            break;
        }
    }
}