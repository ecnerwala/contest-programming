#include <cstdio>
#include <vector>
#include <cassert>

#include "molecules.h"

int main() {
    int n, l, u;
    assert(3 == scanf("%d %d %d", &n, &l, &u));
    std::vector<int> w(n);
    for (int i = 0; i < n; i++)
        assert(1 == scanf("%d", &w[i]));
    std::vector<int> result = find_subset(l, u, w);
    
    
    printf("%d\n", (int)result.size());
    for (int i = 0; i < (int)result.size(); i++)
        printf("%d%c", result[i], " \n"[i == (int)result.size() - 1]);
}
