#include "routers.h"
#include <vector>
#include <cassert>
#include <cstdio>
#include <algorithm>

static int l, n, q;
static std::vector<int> p;
static std::vector<int> answer;
static int queries = 0;

int use_detector(int x)
{
	queries++;
    assert(x >= 0 && x <= l);
    std::vector<int>::iterator left, right;
    right = std::upper_bound(p.begin(), p.end(), x);
    left = std::prev(right);

    if (right == p.end()) {
        return n - 1;
    } else if ((x - *left) <= (*right - x)){
        return std::distance(p.begin(), left);
    } else {
        return std::distance(p.begin(), right);
    }
}

int main()
{
    assert(scanf("%d %d %d", &l, &n, &q) == 3);
    p.resize(n);
    for (int i = 0; i < n; i++) {
        assert(scanf("%d", &p[i]) == 1);
        if (i) {
            assert(p[i-1] < p[i]);
        }
    }

    fclose(stdin);

    answer = find_routers(l, n, q);

    assert((int) answer.size() == n);
    for (int i = 0; i < n; i++)
    {
        assert(answer[i] >= 0 && answer[i] <= l && answer[i] % 2 == 0);
    }

    for (int i = 0; i < n; i++)
    {
        if (i) printf(" ");
        printf("%d", answer[i]);
    }
    printf("\n");

    printf("%d\n", queries);
}
