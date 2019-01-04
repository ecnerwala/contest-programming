// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfStones() {
  return 100000LL;
}

long long GetJumpLength(long long stone) {
  if (stone % 87 == 0) {
    return 150;
  } else {
    return 1;
  }
}
