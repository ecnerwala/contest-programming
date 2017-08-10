// Sample input 1, in CPP.

#include <cassert>

long long GetRows() {
  return 3LL;
}

long long GetColumns() {
  return 4LL;
}

char GetDirection(long long i, long long j) {
  if (i == 0 && j == 0) return '<';
  if (i == 0 && j == 1) return 'v';
  if (i == 0 && j == 2) return '>';
  if (i == 0 && j == 3) return '<';
  if (i == 1 && j == 0) return '<';
  if (i == 1 && j == 1) return '<';
  if (i == 1 && j == 2) return 'v';
  if (i == 1 && j == 3) return '>';
  if (i == 2 && j == 0) return '>';
  if (i == 2 && j == 1) return '>';
  if (i == 2 && j == 2) return '<';
  if (i == 2 && j == 3) return '>';
  assert(0);
}
