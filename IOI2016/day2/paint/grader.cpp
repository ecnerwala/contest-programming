#include "paint.h"

#include <vector>
#include <string>
#include <cstdio>
#include <cassert>

const int S_MAX_LEN = 200 * 1000;
char buf[S_MAX_LEN + 1];

int main() {
    assert(1 == scanf("%s", buf));
    std::string s = buf;
    int c_len;
    assert(1 == scanf("%d", &c_len));
    std::vector<int> c(c_len);
    for (int i = 0; i < c_len; i++) {
        assert(1 == scanf("%d", &c[i]));
    }
    std::string ans = solve_puzzle(s, c);


    printf("%s\n", ans.data());
    return 0;
}
