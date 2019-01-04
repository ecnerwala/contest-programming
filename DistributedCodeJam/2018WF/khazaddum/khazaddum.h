// dummy file
#include <cassert>
#include "message.h"

long long GetRangeLength() {
    return 1000000 * NumberOfNodes();
}

long long GramsToExcavate() {
    return 1;
}

long long GetHeight(long long position) {
    assert(0 <= position && position <= GetRangeLength());
    return 10000;
}
