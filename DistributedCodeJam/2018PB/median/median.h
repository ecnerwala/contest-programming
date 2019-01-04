// dummy file
#include <cassert>
#include "message.h"

namespace judge {

int ceillog2(int i) {
    return i == 1 ? 0 : (1+ceillog2((i+1)/2));
}

const bool large = true;
//const int N = 123456789;
const int N = 12345;
int power;
const int add = 1098255 % N;
const int add2 = 109725 % N;
const int m1 = (9129865 % N) | 1;
const int m2 = (897235 % N) | 1;

int GetN() {
    if (large) {
        return -1;
    } else {
        return N;
    }
}

int GetP(int i) {
    unsigned long long index = (unsigned long long) i;
    if (power == 0) power = ceillog2(N);
    assert(power >= 2);
    do {
        unsigned long long MASK = ((1ull << power) - 1);
        index += add;
        index *= m1;
        index &= MASK;
        index ^= index >> ((power / 2) + 1);
        index += add2;
        index *= m2;
        index &= MASK;
        index ^= index >> (2 * power / 3);
    } while (index >= N);
    return int(index);
}

int GetData(long long i) {
    long long d = large ? (N / NumberOfNodes()) * MyNodeId() : 0;

    int index = GetP(int((i+d) % N));
    return index % 2 + 1;
    return index+1;

    long long v = index;
    v *= index;
    return int(v % (int(1e9) + 9));
}

}

using judge::GetN;
using judge::GetData;
