// dummy file
#include <cassert>
#include "message.h"

long long GetStackSize() {
    return 1;
}
long long GetNumDiners() {
    return 1;
}
long long GetStackItem(long long i) {
    return i % GetNumDiners();
}
