// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfStones() {
    return 100;
}

long long GetJumpLength(long long location) {
    assert(0 <= location && location <= GetNumberOfStones());
    return 1;
}
