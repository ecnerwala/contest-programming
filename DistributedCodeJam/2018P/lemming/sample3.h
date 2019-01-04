// Sample input 3, in CPP.

#include <cassert>

long long GetRows() {
  return 4LL;
}

long long GetColumns() {
  return 3LL;
}

char GetDirection(long long i, long long j) {
  if (i == 0 && j == 0) return '<';
  if (i == 0 && j == 1) return 'v';
  if (i == 0 && j == 2) return '<';
  if (i == 1 && j == 0) return '>';
  if (i == 1 && j == 1) return '>';
  if (i == 1 && j == 2) return '^';
  if (i == 2 && j == 0) return 'v';
  if (i == 2 && j == 1) return '>';
  if (i == 2 && j == 2) return '>';
  if (i == 3 && j == 0) return '^';
  if (i == 3 && j == 1) return '^';
  if (i == 3 && j == 2) return '^';
  assert(0);
}
