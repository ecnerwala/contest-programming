// Sample input 2, in CPP.

#include <cassert>

#include <cstdio>

long long GetN() {
  return 5LL;
}

long long GetSpawningFrequency() {
  return 1LL;
}

long long RowGeneratorPosition(long long row) {
  switch ((int)row) {
    case 0: return 5LL;
    case 1: return 5LL;
    case 2: return 5LL;
    case 3: return 5LL;
    case 4: return 5LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "row = %lld\n", row);
      assert(0);
  }
}

long long ColumnGeneratorPosition(long long column) {
  switch ((int)column) {
    case 0: return 5LL;
    case 1: return 5LL;
    case 2: return 5LL;
    case 3: return 5LL;
    case 4: return 5LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "column = %lld\n", column);
      assert(0);
  }
}
