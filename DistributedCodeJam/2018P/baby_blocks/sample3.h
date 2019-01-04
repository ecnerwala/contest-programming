// Sample input 3, in CPP.

#include <cassert>

long long GetNumberOfBlocks() {
  return 10LL;
}

long long GetBlockWeight(long long i) {
  switch ((int)i) {
    case 0: return 1LL;
    case 1: return 2LL;
    case 2: return 4LL;
    case 3: return 8LL;
    case 4: return 16LL;
    case 5: return 32LL;
    case 6: return 16LL;
    case 7: return 8LL;
    case 8: return 4LL;
    case 9: return 2LL;
    default: assert(0);
  }
}
