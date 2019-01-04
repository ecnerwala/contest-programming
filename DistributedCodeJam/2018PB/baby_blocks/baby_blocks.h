// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfBlocks() {
    return 2000;
}

long long GetBlockWeight(long long i) {
    assert(0 <= i && i < GetNumberOfBlocks());
    return 1;
}
