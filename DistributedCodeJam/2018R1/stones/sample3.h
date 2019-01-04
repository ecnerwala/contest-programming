// Sample input 3, in CPP.

#include <cassert>

#include <cstdio>

long long GetNumberOfStones() {
  return 10LL;
}

long long GetJumpLength(long long stone) {
  switch ((int)stone) {
    case 0: return 3LL;
    case 1: return 4LL;
    case 2: return 2LL;
    case 3: return 3LL;
    case 4: return 4LL;
    case 5: return 2LL;
    case 6: return 3LL;
    case 7: return 4LL;
    case 8: return 2LL;
    case 9: return 3LL;
    case 10: return 4LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "stone = %lld\n", stone);
      assert(0);
  }
}
