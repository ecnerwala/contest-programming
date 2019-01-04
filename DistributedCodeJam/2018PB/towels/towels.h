// dummy file
#include <cassert>
#include "message.h"

long long GetNumberOfStacks() {
    return 1;
}

long long GetNumberOfTowelsInStack() {
    return 1;
}

long long GetNumberOfGymUsersBeforeYou() {
    return 0;
}

long long GetTowelCleanlinessRank(long long stack, long long position) {
    return stack * GetNumberOfTowelsInStack() + position;
}
