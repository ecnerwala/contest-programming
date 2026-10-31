#include <bits/stdc++.h>

int main() {
    using namespace std;
    struct pt_t {
        int cnt;
        array<int, 2> x;
        char c;
    };
    vector<pair<array<int, 2>, array<pt_t, 3>>> cases;
    for (int N = 3; N <= 8; N++) {
        for (int M = 3; M <= 8; M++) {
            for (int c0 = N*M; c0 >= 1; c0--) {
                for (int c1 = min(c0, N*M-c0); c1 >= 1; c1--) {
                    int c2 = N*M-c0-c1;
                    if (c2 == 0) continue;
                    if (c2 > c1) break;
                    for (int x1 = 0; x1 < N; x1++) {
                        for (int y1 = 0; y1 < M; y1++) {
                            if (x1==0 && y1==0) continue;
                            for (int x2 = 0; x2 < N; x2++) {
                                for (int y2 = 0; y2 < M; y2++) {
                                    if (x2 == 0 && y2 == 0) continue;
                                    if (x1 == x2 && y1 == y2) continue;
                                    cases.push_back({array<int, 2>{N, M}, array<pt_t, 3>{
                                        pt_t{c0, {0,0}, 'A'},
                                        pt_t{c1, {x1,y1}, 'B'},
                                        pt_t{c2, {x2,y2}, 'C'}
                                    }});
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    cout << cases.size() << '\n';
    for (const auto& [n, c] : cases) {
        cout << n[0] << ' ' << n[1] << '\n';
        cout << c[0].cnt << ' ' << c[1].cnt << ' ' << c[2].cnt << '\n';
        for (auto& p : c) cout << p.x[0]+1 << ' ' << p.x[1]+1 << '\n';
    }
}
