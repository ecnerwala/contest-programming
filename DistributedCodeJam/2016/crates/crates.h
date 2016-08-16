// Sample input 3, in CPP.

#include <cassert>

long long GetNumStacks() {
  return 3LL;
}

long long GetStackHeight(long long i) {
  switch ((int)i) {
    case 1: return 2LL;
    case 2: return 2LL;
    case 3: return 2LL;
    default: assert(0);
  }
}
