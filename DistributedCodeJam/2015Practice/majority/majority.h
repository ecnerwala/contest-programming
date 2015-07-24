// Sample input 3, in CPP.

#include <cassert>

long long GetN() {
  return 5LL;
}

long long GetVote(long long i) {
  switch ((int)i) {
    case 0: return 2LL;
    case 1: return 2LL;
    case 2: return 3LL;
    case 3: return 4LL;
    case 4: return 5LL;
    default: assert(0);
  }
}
