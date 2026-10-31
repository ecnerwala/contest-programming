#include "ckollib.h"

int main(void) {
  int wynik = -1;
  while (1) {
    int k = karta();
    if (k == 0) {
      break;
    }
    wynik = k;
  }
  odpowiedz(wynik);
  return 0;
}

