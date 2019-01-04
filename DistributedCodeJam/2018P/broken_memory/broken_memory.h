// dummy file
#include <cassert>
#include "message.h"

long long GetLength() {
    return 1000;
}

long long GetValue(long long i) {
    if (i == MyNodeId()) {
        return 2;
    } else {
        return 1;
    }
}
