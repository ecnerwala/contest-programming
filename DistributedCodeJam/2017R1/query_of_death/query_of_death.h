// Sample input 3, in CPP.
#include <cassert>
#include <stdlib.h>
#include "message.h"

static int isthenodebroken = 0;
static int testvs[] = {1, 0, 1, 0, 1};

long long GetLength() {
  return 5LL;
}

long long GetValue(long long i) {
  if (i < 0LL || i >= GetLength())
    assert(0);
  int val = (int)(testvs[i]^(rand() % (isthenodebroken + 1)));
  if (i == 4)
    isthenodebroken = 1;
  return val;
}
