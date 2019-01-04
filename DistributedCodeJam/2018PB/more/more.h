// dummy file
#include <cassert>
#include "message.h"

long long GetN() {
    return 100;
}
long long GetP() {
    return 1000000007;
}
long long GetM() {
    return 1 % GetP();
}
long long GetA(long long i) {
    assert(3 <= i && i <= GetN());
    return i % GetP();
}
