// Sample input 2, in CPP.
#include <cassert>
#include <stdlib.h>
#include "message.h"

static int isthenodebroken = 0;
static int testvs[] = {1, 1, 1};

long long GetLength() {
  return 3LL;
}

int GetValue(long long i) {
  if (i < 0LL || i >= GetLength())
    assert(0);
  int val = (int)(testvs[i]^(rand() % (isthenodebroken + 1)));
  if (i == 0)
    isthenodebroken = 1;
  return val;
}
