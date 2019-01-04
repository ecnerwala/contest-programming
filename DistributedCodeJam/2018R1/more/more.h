// dummy file
#include <cassert>
#include "message.h"

long long GetN() {
    return 1;
}

long long GetP() {
    return 1000000;
}

long long GetM() {
    return 2;
}

long long GetA(long long i) {
    return i % GetP();
}
