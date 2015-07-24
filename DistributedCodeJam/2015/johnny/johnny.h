// Sample input 1, in CPP.

#include <cassert>

long long NumberOfCards() {
  return 4LL;
}

bool IsBetter(long long i, long long j) {
  if (i == 0 && j == 0) return false;
  if (i == 0 && j == 1) return true;
  if (i == 0 && j == 2) return true;
  if (i == 0 && j == 3) return true;
  if (i == 1 && j == 0) return false;
  if (i == 1 && j == 1) return false;
  if (i == 1 && j == 2) return false;
  if (i == 1 && j == 3) return true;
  if (i == 2 && j == 0) return false;
  if (i == 2 && j == 1) return true;
  if (i == 2 && j == 2) return false;
  if (i == 2 && j == 3) return false;
  if (i == 3 && j == 0) return false;
  if (i == 3 && j == 1) return false;
  if (i == 3 && j == 2) return true;
  if (i == 3 && j == 3) return false;
  assert(0);
}
