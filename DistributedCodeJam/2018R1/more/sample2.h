// Sample input 2, in CPP.

#include <cassert>

#include <cstdio>

long long GetN() {
  return 4LL;
}

long long GetP() {
  return 2LL;
}

long long GetM() {
  return 0LL;
}

long long GetA(long long i) {
  switch ((int)i) {
    case 3: return 0LL;
    case 4: return 0LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "i = %lld\n", i);
      assert(0);
  }
}
