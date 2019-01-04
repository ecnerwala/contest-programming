// Sample input 2, in CPP.

#include <cassert>

long long GetLength() {
  return 3LL;
}

long long GetDigitX(long long i) {
  switch ((int)i) {
    case 0: return 1000LL;
    case 1: return 20000LL;
    case 2: return 300000LL;
    default: assert(0);
  }
}

long long GetDigitY(long long i) {
  switch ((int)i) {
    case 0: return 40000LL;
    case 1: return 500000LL;
    case 2: return 6LL;
    default: assert(0);
  }
}

long long GetDigitZ(long long i) {
  switch ((int)i) {
    case 0: return 41000LL;
    case 1: return 520000LL;
    case 2: return 300006LL;
    default: assert(0);
  }
}
