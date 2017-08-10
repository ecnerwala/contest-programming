// Sample input 2, in CPP.
#include <cassert>
#include "message.h"

static int _the_data_[] = {1000000000};
static int _the_delta_[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int GetN() {
  return 1;
}

int GetData(long long i) {
  if (i < 0LL || i > 1000000000000000000LL)
    assert(0);
  return _the_data_[(((int)i) + 0 * _the_delta_[MyNodeId()]) % 1];
}

