// Large input

#include <cassert>

long long GetRows() {
  return 3000LL;
}

long long GetColumns() {
  return 3000LL;
}

char GetDirection(long long i, long long j) {
  return "<>^v"[(i+j)%4];
}
