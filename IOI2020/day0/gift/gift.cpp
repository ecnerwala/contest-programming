#include "gift.h"

#include <vector>
#include <array>
#include <cassert>

int construct(int N, int R, std::vector<int> A, std::vector<int> B, std::vector<int> X) {
    std::vector<std::array<int, 2>> bounds(N, {-1, N});
    for (int i = 0; i < N; i++) bounds[i][0] = i;
    for (int r = 0; r < R; r++) {
        int a = A[r], b = B[r], x = X[r];
        assert(0 <= a && a <= b && b <= N-1);
        if (x == 1) {
            bounds[a][0] = std::max(bounds[a][0], b);
        } else if (x == 2) {
            bounds[a][1] = std::min(bounds[a][1], b);
        } else return false;
    }
    std::string S(N, '\0');
    bool cur = 0;
    for (int i = 0; i < N; i++) {
        S[i] = "RB"[cur];
        if (bounds[i][0] >= bounds[i][1]) return false;
        if (bounds[i][0] == i) {
            // we can switch
            cur ^= 1;
        } else {
            bounds[i+1][0] = std::max(bounds[i+1][0], bounds[i][0]);
            bounds[i+1][1] = std::min(bounds[i+1][1], bounds[i][1]);
        }
    }
    craft(S); return true;
}
