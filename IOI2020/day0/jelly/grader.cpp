#include "jelly.h"
#include <cstdio>
#include <cassert>
#include <vector>

static int n, x, y;
static std::vector<int> a, b;

int main() {
    assert(scanf("%d %d %d", &n, &x, &y) == 3);
    a.resize(n);
    b.resize(n);
    for (int i = 0; i < n; i++) {
        assert(scanf("%d %d", &a[i], &b[i]) == 2);
    }

    fclose(stdin);

    int answer = find_maximum_unique(x, y, a, b);

    printf("%d\n", answer);
}
