// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfStones() {
    return 1;
}

long long GetJumpLength(long long location) {
    return (location + 1) - location;
}
