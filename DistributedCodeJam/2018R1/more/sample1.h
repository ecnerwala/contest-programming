// Sample input 1, in CPP.

#include <cassert>

#include <cstdio>

long long GetN() {
  return 3LL;
}

long long GetP() {
  return 10LL;
}

long long GetM() {
  return 0LL;
}

long long GetA(long long i) {
  switch ((int)i) {
    case 3: return 9LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "i = %lld\n", i);
      assert(0);
  }
}
