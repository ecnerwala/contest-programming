#include "aliens.h"

#include <cstdio>
#include <cassert>

int main() {
    int n, m, k;
    assert(3 == scanf("%d %d %d", &n, &m, &k));
    std::vector<int> r(n), c(n);
    for (int i = 0; i < n; i++) {
        assert(2 == scanf("%d %d", &r[i], &c[i]));
    }
    long long ans = take_photos(n, m, k, r, c);
    
    
    printf("%lld\n", ans);
    return 0;
}
