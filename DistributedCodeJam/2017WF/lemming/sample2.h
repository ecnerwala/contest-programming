// Sample input 2, in CPP.

#include <cassert>

long long GetRows() {
  return 1LL;
}

long long GetColumns() {
  return 15LL;
}

char GetDirection(long long i, long long j) {
  if (i == 0 && j == 0) return '>';
  if (i == 0 && j == 1) return '<';
  if (i == 0 && j == 2) return '>';
  if (i == 0 && j == 3) return '<';
  if (i == 0 && j == 4) return '>';
  if (i == 0 && j == 5) return '<';
  if (i == 0 && j == 6) return '>';
  if (i == 0 && j == 7) return '<';
  if (i == 0 && j == 8) return '>';
  if (i == 0 && j == 9) return '<';
  if (i == 0 && j == 10) return '>';
  if (i == 0 && j == 11) return '<';
  if (i == 0 && j == 12) return '>';
  if (i == 0 && j == 13) return '<';
  if (i == 0 && j == 14) return '>';
  assert(0);
}
