// Sample input 3, in CPP.

#include <cassert>

#include <cstdio>

long long GetN() {
  return 3LL;
}

long long GetSpawningFrequency() {
  return 6LL;
}

long long RowGeneratorPosition(long long row) {
  switch ((int)row) {
    case 0: return 4LL;
    case 1: return 3LL;
    case 2: return 8LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "row = %lld\n", row);
      assert(0);
  }
}

long long ColumnGeneratorPosition(long long column) {
  switch ((int)column) {
    case 0: return 8LL;
    case 1: return 8LL;
    case 2: return 8LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "column = %lld\n", column);
      assert(0);
  }
}
