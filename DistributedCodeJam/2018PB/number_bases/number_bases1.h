// Sample input 1, in CPP.

#include <cassert>

long long GetLength() {
  return 3LL;
}

long long GetDigitX(long long i) {
  switch ((int)i) {
    case 0: return 3LL;
    case 1: return 2LL;
    case 2: return 1LL;
    default: assert(0);
  }
}

long long GetDigitY(long long i) {
  switch ((int)i) {
    case 0: return 6LL;
    case 1: return 5LL;
    case 2: return 4LL;
    default: assert(0);
  }
}

long long GetDigitZ(long long i) {
  switch ((int)i) {
    case 0: return 0LL;
    case 1: return 8LL;
    case 2: return 5LL;
    default: assert(0);
  }
}
