// Sample input 3, in CPP.

#include <cassert>

const long long __SECRET_LEN = 1e9;
long long GetNumberLength() {
  return __SECRET_LEN;
}

long long GetDigit(long long i) {
  assert(__SECRET_LEN % 10 == 0);
  return 9 - i / (__SECRET_LEN / 10);
}
