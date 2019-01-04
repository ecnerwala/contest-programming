// dummy file
#include <cassert>
#include "message.h"

long long GetN() {
    return 30000;
}
long long GetSpawningFrequency() {
    return 1;
}
long long RowGeneratorPosition(long long row) {
    assert(0 <= row && row < GetN());
    return GetN();
}
long long ColumnGeneratorPosition(long long column) {
    assert(0 <= column && column < GetN());
    return GetN();
}
