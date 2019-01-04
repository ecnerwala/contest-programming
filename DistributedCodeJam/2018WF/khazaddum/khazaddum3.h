// Sample input 3, in CPP.

#include <cassert>

#include <cstdio>

long long GetRangeLength() {
  return 5LL;
}

long long GramsToExcavate() {
  return 1LL;
}

long long GetHeight(long long position) {
  switch ((int)position) {
    case 0: return 3LL;
    case 1: return 0LL;
    case 2: return 1LL;
    case 3: return 1LL;
    case 4: return 0LL;
    case 5: return 3LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "position = %lld\n", position);
      assert(0);
  }
}
