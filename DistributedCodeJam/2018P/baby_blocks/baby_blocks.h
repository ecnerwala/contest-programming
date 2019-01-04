// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfBlocks() {
    return 2;
}

long long GetBlockWeight(long long i) {
    return "\1\1"[i];
}
