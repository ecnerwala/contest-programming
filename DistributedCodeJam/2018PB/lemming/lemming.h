// dummy file
#include <cassert>
#include "message.h"

long long GetRows() {
    return 100;
}

long long GetColumns() {
    return 2;
}

char GetDirection(long long r, long long c) {
    assert(0 <= r && r < GetRows());
    assert(0 <= c && c < GetColumns());
    return "^v<>"[(__builtin_popcount(unsigned(r))&1)<<1 | (__builtin_popcount(unsigned(c))&1)];
}
