// Sample input 2, in C.

#include <assert.h>

#include <stdbool.h>

long long NumberOfFiles() {
  return 4LL;
}

long long MaxDistance() {
  return 2LL;
}

long long Identifier(long long i) {
  switch ((int)i) {
    case 0: return 1000LL;
    case 1: return 1500LL;
    case 2: return 0LL;
    case 3: return 500LL;
    default: assert(0);
  }
}
