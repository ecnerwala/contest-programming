// dummy file
#include <cassert>
#include "message.h"

int GetLength() {
    return 1;
}

char GetCharacter(long long i) {
    assert(0 <= i && i < 1);
    return '(';
}
