#include <bits/stdc++.h>
using namespace std;

const long long INF = 1e18;

const int MAXN = 200;
int N;
long long W[MAXN][MAXN];

int numE[MAXN][MAXN];

long long ans[MAXN][MAXN][MAXN];

pair<long long, pair<int, int>> edges[MAXN*MAXN];

int main() {
    cin >> N;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> W[i][j];
            if (W[i][j] == 0) W[i][j] = INF;
            if (i == j) W[i][j] = 0;
        }
    }
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                W[i][j] = min(W[i][j], W[i][k] + W[k][j]);
            }
        }
    }
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            edges[i*N+j].first = W[i][j];
            edges[i*N+j].second = pair<int, int>(i, j);
        }
    }
    sort(edges, edges + (N*N));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            numE[i][j] = (i == j) ? 0 : N; // inf
        }
    }
    
    for (int e = 0; e < N * N; e++) {
        long long w = edges[e].first;
        int i = edges[e].second.first;
        int j = edges[e].second.second;
        for (int a = 0; a < N; a++) {
            for (int b = 0; b < N; b++) {
                int l = numE[a][i] + 1 + numE[j][b];
                while (l < numE[a][b]) {
                    ans[a][b][--numE[a][b]] = w;
                }
            }
        }
    }

    int Q; cin >> Q;
    for (int q = 0; q < Q; q++) {
        int s, t, d; cin >> s >> t >> d; s--, t--;
        assert(s != t);
        assert(0 <= s && s < N);
        assert(0 <= t && t < N);
        assert(1 <= d && d < N);

        assert(numE[s][t] == 1);
        long long res = ans[s][t][d];
        if (res == INF) res = 0;
        cout << res << '\n';
    }
    
    return 0;
}
