// Sample input 1, in CPP.
#include <cassert>
#include "message.h"

static int _the_data_[] = {5,9,4,8,11,7,10,6,1,2,3};
static int _the_delta_[] = {0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0,1,2,3,4,5,6,7,8,9,10,0};

int GetN() {
  return 11;
}

int GetData(long long i) {
  if (i < 0LL || i > 1000000000000000000LL)
    assert(0);
  return _the_data_[(((int)i) + 0 * _the_delta_[MyNodeId()]) % 11];
}

