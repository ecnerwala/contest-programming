#include "railroad.h"
#include <cstdio>
#include <cassert>

int main() {
    int n;
    assert(1 == scanf("%d", &n));
    std::vector<int> s(n), t(n);
    for (int i = 0; i < n; ++i)
        assert(2 == scanf("%d%d", &s[i], &t[i]));
    long long ans = plan_roller_coaster(s, t);
    printf("%lld\n", ans);
    return 0;
}

