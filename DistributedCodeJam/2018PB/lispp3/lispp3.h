// dummy file
#include <cassert>
#include "message.h"

int GetLength() {
    return 7;
}

char GetCharacter(long long i) {
    assert(0 <= i && i < GetLength());
    return "(()+())"[i];
}
