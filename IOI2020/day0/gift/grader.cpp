#include "gift.h"
#include <vector>
#include <cassert>
#include <cstdio>
#include <string>

static int n, r;
static std::vector<int> a, b, x;
static std::string s;
static int possible = 0;
static int called = 0;

void craft(std::string &_s)
{
    assert(!called);
    s = _s;
    assert((int) s.size() == n);
    for (int i = 0; i < n; i++) {
        assert(s[i] == 'R' || s[i] == 'B');
    }
    called = 1;
}

int main()
{
    assert(scanf("%d %d", &n, &r) == 2);
    a.resize(r);
    b.resize(r);
    x.resize(r);
    for (int i = 0; i < r; i++) {
        assert(scanf("%d %d %d", &a[i], &b[i], &x[i]) == 3);
    }

    fclose(stdin);

    possible = construct(n, r, a, b, x);

    assert(possible == 0 || possible == 1);

    printf("%d\n", possible);

    if (possible == 1) {
        printf("%s\n", s.c_str());
    } else {
        assert(!called);
    }

}
