#include "candies.h"

#include <cassert>
#include <cstdio>
#include <vector>

int main() {
    int n;
    assert(1 == scanf("%d", &n));
    std::vector<int> c(n);
    for(int i = 0; i < n; ++i) {
        assert(scanf("%d", &c[i]) == 1);
    }

    int q;
    assert(1 == scanf("%d", &q));
    std::vector<int> l(q), r(q), v(q);
    for(int i = 0; i < q; ++i) {
        assert(scanf("%d %d %d", &l[i], &r[i], &v[i]) == 3);
    }

    std::vector<int> ans = distribute_candies(c, l, r, v);

    for(int i = 0; i < n; ++i) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", ans[i]);
    }
    printf("\n");
    fclose(stdout);
    return 0;
}
