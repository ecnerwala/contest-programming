#include <bits/stdc++.h>

int main() {
    using namespace std;
    int N, M; cin >> N >> M;
    vector<array<int, 2>> edges(M);
    for (auto& e : edges) cin >> e[0] >> e[1];

    for (int k = 1; k <= 500; k++) {
        int E;
        assert(cin >> E);
        vector<int> bads(20);
        for (int i = 0; i < E; i++) {
            int a, b; cin >> a >> b;
            assert(4 <= a && a <= N);
            assert(1 <= b && b <= 3);
            bads[a] |= (1 << (b-1));
        }
        array<int, 3> dp{0,1,0};
        for (int i = 4; i+1 <= N; i += 2) {
            array<int, 3> ndp{0, 0, 0};
            for (int a = 0; a < 3; a++) {
                for (int b = 0; b < 3; b++) {
                    for (int c = 0; c < 3; c++) {
                        if (a == b) continue;
                        if (b == c) continue;
                        if (bads[i] & (1 << b)) continue;
                        if (bads[i+1] & (1 << c)) continue;
                        ndp[b] += dp[a];
                    }
                }
            }
            dp = ndp;
        }
        int tot = dp[0] + dp[1] + dp[2];
        if (tot != k) {
            cerr << "Bad: Expected " << k << " got " << tot << '\n';
        } else {
            cerr << "Good: Expected " << k << " got " << tot << '\n';
        }
    }
}
