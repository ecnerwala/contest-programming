// Sample input 3, in CPP.

#include <cassert>

long long GetLength() {
  return 4LL;
}

long long GetDigitX(long long i) {
  switch ((int)i) {
    case 0: return 5LL;
    case 1: return 3LL;
    case 2: return 0LL;
    case 3: return 0LL;
    default: assert(0);
  }
}

long long GetDigitY(long long i) {
  switch ((int)i) {
    case 0: return 5LL;
    case 1: return 3LL;
    case 2: return 0LL;
    case 3: return 0LL;
    default: assert(0);
  }
}

long long GetDigitZ(long long i) {
  switch ((int)i) {
    case 0: return 0LL;
    case 1: return 6LL;
    case 2: return 0LL;
    case 3: return 0LL;
    default: assert(0);
  }
}
