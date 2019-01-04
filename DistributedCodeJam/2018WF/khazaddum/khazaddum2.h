// Sample input 2, in CPP.

#include <cassert>

#include <cstdio>

long long GetRangeLength() {
  return 3LL;
}

long long GramsToExcavate() {
  return 1LL;
}

long long GetHeight(long long position) {
  switch ((int)position) {
    case 0: return 10LL;
    case 1: return 0LL;
    case 2: return 0LL;
    case 3: return 10LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "position = %lld\n", position);
      assert(0);
  }
}
