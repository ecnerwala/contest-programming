// Sample input 3, in CPP.

#include <cassert>

long long GetN() {
  return 4LL;
}

long long GetTaste(long long index) {
  switch ((int)index) {
    case 0: return 1LL;
    case 1: return 1LL;
    case 2: return 2LL;
    case 3: return 1LL;
    default: assert(0);
  }
}
