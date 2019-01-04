// Sample input 3, in CPP.

#include <cassert>

#include <cstdio>

long long GetN() {
  return 10LL;
}

long long GetP() {
  return 1000000LL;
}

long long GetM() {
  return 1LL;
}

long long GetA(long long i) {
  switch ((int)i) {
    case 3: return 10LL;
    case 4: return 0LL;
    case 5: return 0LL;
    case 6: return 0LL;
    case 7: return 0LL;
    case 8: return 0LL;
    case 9: return 1000LL;
    case 10: return 7LL;
    default:
      fprintf(stderr, "Incorrect value:\n");
      fprintf(stderr, "i = %lld\n", i);
      assert(0);
  }
}
