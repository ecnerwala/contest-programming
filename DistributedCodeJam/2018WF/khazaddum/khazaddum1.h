// Sample input 1, in CPP.

#include <cassert>

#include <cstdio>

long long GetRangeLength() {
  return 2LL;
}

long long GramsToExcavate() {
  return 200LL;
}

long long GetHeight(long long position) {
  switch ((int)position) {
    case 0: return 3LL;
    case 1: return 3LL;
    case 2: return 2LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "position = %lld\n", position);
      assert(0);
  }
}
