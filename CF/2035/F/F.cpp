#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, RT; cin >> N >> RT; RT--;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		int64_t ans = -1;
		const int64_t INF = N + *std::max_element(A.begin(), A.end());
		for (int mod = 0; mod < 2 * N; mod++) {
			int64_t mi = -1;
			int64_t ma = 2 * INF + 1;
			while (ma - mi > 1) {
				int64_t md = (mi + ma) / 2;
				if ([&](this auto&& self, int cur, int prv) -> int64_t {
					// we need the number of ops in this subtree must be the right parity, and also
					int64_t num_need = A[cur];
					for (int nxt : adj[cur]) {
						if (nxt == prv) continue;
						num_need += self(nxt, cur);
					}
					num_need -= (cur < mod) + (cur + N < mod) + 2 * md;
					if (num_need < 0) {
						num_need = num_need & 1;
					}
					return num_need;
				}(RT, -1) == 0) {
					ma = md;
				} else {
					mi = md;
				}
			}
			if (ma < 2 * INF + 1) {
				int64_t cnd = ma * (2 * N) + mod;
				if (ans == -1 || cnd < ans) ans = cnd;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
