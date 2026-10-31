#include<bits/stdc++.h>

int main() {
    using namespace std;
    ios_base::sync_with_stdio(0), cin.tie(nullptr);
    int T; cin >> T;
    for (int cases = 1; cases <= T; cases++) {
        using i64 = int64_t;
        int N, Q; cin >> N >> Q;
        string A; cin >> A;
        vector<i64> L(N); for (int i = 0; i < N; i++) cin >> L[i];
        vector<i64> R(N); for (int i = 0; i < N; i++) cin >> R[i];
        vector<i64> P(N); for (int i = 0; i < N; i++) cin >> P[i];

        vector<int> match(N);
        {
            stack<int> stk;
            for (int i = 0; i < N; i++) {
                if (A[i] == '(') {
                    stk.push(i);
                } else if (A[i] == ')') {
                    match[stk.top()] = i;
                    match[i] = stk.top();
                    stk.pop();
                } else assert(false);
            }
        }

        vector<vector<i64>> dist(N);
        for (int i = 0; i < N; i++) {
            priority_queue<pair<i64, int>, vector<pair<i64, int>>, greater<pair<i64, int>>> pq;
            dist[i] = vector<i64>(N, -1);
            auto check = [&](int nxt, i64 nd) {
                if (dist[i][nxt] == -1 || nd < dist[i][nxt]) {
                    dist[i][nxt] = nd;
                    pq.emplace(nd, nxt);
                }
            };
            check(i, 0);
            while (!pq.empty()) {
                i64 d = pq.top().first; int cur = pq.top().second; pq.pop();
                if (d != dist[i][cur]) continue;
                check(max(cur-1, 0), d + L[cur]);
                check(min(cur+1, N-1), d + R[cur]);
                check(match[cur], d + P[cur]);
            }
        }

        vector<int> S(Q), E(Q);
        for (int i = 0; i < Q; i++) cin >> S[i], S[i]--;
        for (int i = 0; i < Q; i++) cin >> E[i], E[i]--;

#ifdef ECNERWALA
        cout << "Case #" << cases << ":" << '\n';
        for (int i = 0; i < Q; i++) {
            cout << dist[S[i]][E[i]] << '\n';
        }
#else
        i64 ans = 0;
        for (int i = 0; i < Q; i++) {
            ans += dist[S[i]][E[i]];
        }

        cout << "Case #" << cases << ": " << ans << '\n';
#endif
    }
}
