// Sample input 1, in CPP.

#include <cassert>

#include <cstdio>

long long GetNumberOfStones() {
  return 1LL;
}

long long GetJumpLength(long long stone) {
  switch ((int)stone) {
    case 0: return 1LL;
    case 1: return 2LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "stone = %lld\n", stone);
      assert(0);
  }
}
