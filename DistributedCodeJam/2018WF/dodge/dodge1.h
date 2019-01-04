// Sample input 1, in CPP.

#include <cassert>

#include <cstdio>

long long GetN() {
  return 2LL;
}

long long GetSpawningFrequency() {
  return 3LL;
}

long long RowGeneratorPosition(long long row) {
  switch ((int)row) {
    case 0: return 2LL;
    case 1: return 3LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "row = %lld\n", row);
      assert(0);
  }
}

long long ColumnGeneratorPosition(long long column) {
  switch ((int)column) {
    case 0: return 2LL;
    case 1: return 4LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "column = %lld\n", column);
      assert(0);
  }
}
