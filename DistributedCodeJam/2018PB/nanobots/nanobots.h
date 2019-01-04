// dummy file
#include <cassert>
#include "message.h"

long long GetNumNanobots() {
    return 20;
}
long long GetRange() {
    return 10;
}
char Experiment(long long c, long long d) {
    assert(1 <= c && c <= GetRange());
    assert(1 <= d && d <= GetRange());
    if (c + d <= GetRange() + 1) {
        return 'T';
    } else {
        return 'E';
    }
}
