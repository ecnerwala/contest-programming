#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr);
    mt19937 mt(48);

    int T = 2;
    cout << T << '\n';
    for (int t = 0; t < T; t++) {
        int N = int(1e5);
        int M = int(2e5);
        int Q = int(1e5);
        cout << N << ' ' << M << ' ' << Q << '\n';
        uniform_int_distribution<int> gen_vert(1, N);
        uniform_int_distribution<int> gen_weight(1, int(1e9));
        for (int e = 0; e < M; e++) {
            int u = gen_vert(mt);
            int v = u; while (v == u) v = gen_vert(mt);
            cout << u << ' ' << v << ' ' << gen_weight(mt) << '\n';
        }
        for (int q = 0; q < Q; q++) {
            int u = gen_vert(mt);
            int v = u; while (v == u) v = gen_vert(mt);
            cout << u << ' ' << v << '\n';
        }
    }
}
