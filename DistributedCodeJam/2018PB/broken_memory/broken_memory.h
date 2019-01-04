// dummy file
#include <cassert>
#include "message.h"

long long GetLength() {
    return 100;
}

long long GetValue(long long i) {
    long long v = i;
    if (i == MyNodeId()) {
        v++;
    }
    return v;
}
