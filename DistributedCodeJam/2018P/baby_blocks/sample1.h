// Sample input 1, in CPP.

#include <cassert>

long long GetNumberOfBlocks() {
  return 3LL;
}

long long GetBlockWeight(long long i) {
  switch ((int)i) {
    case 0: return 1LL;
    case 1: return 1LL;
    case 2: return 1LL;
    default: assert(0);
  }
}
