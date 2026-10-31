#include "keys.h"
#include <cassert>
#include <cstdio>

int main() {
    int n, m;
    assert(2 == scanf("%d%d", &n, &m));
    std::vector<int> r(n), u(m), v(m), c(m);
    for(int i=0; i<n; i++) {
        assert(1 == scanf("%d", &r[i]));
    }
    for(int i=0; i<m; i++) {
        assert(3 == scanf("%d %d %d", &u[i], &v[i], &c[i]));
    }
    fclose(stdin);

    std::vector<int> ans = find_reachable(r, u, v, c);

    for (int i = 0; i < (int) ans.size(); ++i) {
        if(i) putchar(' ');
        putchar(ans[i]+'0');
    }
    printf("\n");
    return 0;
}
