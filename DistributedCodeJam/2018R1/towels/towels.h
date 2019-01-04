// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfStacks() {
    return 1;
}

long long GetNumberOfTowelsInStack() {
    return 2;
}

long long GetNumberOfGymUsersBeforeYou() {
    return 1;
}

long long GetTowelCleanlinessRank(long long stack, long long position) {
    assert(0 <= stack && stack < 1);
    assert(0 <= position && position < 2);
    return (position == 0) ? 2 : 1;
}
