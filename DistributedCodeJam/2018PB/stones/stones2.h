// Sample input 2, in CPP.

#include <cassert>

#include <cstdio>

long long GetNumberOfStones() {
  return 5LL;
}

long long GetJumpLength(long long stone) {
  switch ((int)stone) {
    case 0: return 3LL;
    case 1: return 1LL;
    case 2: return 10LL;
    case 3: return 1LL;
    case 4: return 1LL;
    case 5: return 1LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "stone = %lld\n", stone);
      assert(0);
  }
}
