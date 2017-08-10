// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfBlocks() {
    return 2;
}

long long GetBlockWeight(long long i) {
    assert(0 <= i && i < 2);
    return 1;
    return i + 1;
}
