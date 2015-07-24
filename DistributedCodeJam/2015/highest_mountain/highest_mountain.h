// Sample input 3, in CPP.

#include <cassert>

int NumberOfPeaks() {
  return 5;
}

int GetHeight(long long i) {
  switch ((int)i) {
    case 0: return 1;
    case 1: return 3;
    case 2: return 4;
    case 3: return 2;
    case 4: return 1;
    default: assert(0);
  }
}
