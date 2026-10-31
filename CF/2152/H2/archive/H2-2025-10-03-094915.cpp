#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_CASES; cin >> N_CASES;
	while (N_CASES--) {
		int N; cin >> N;
		std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; int64_t w; cin >> u >> v >> w; u--, v--;
			adj[u].push_back({v, w});
			adj[v].push_back({u, w});
		}

		int Q; cin >> Q;
		while (Q--) {
			int64_t t; cin >> t;
			int64_t ans = 0;
			[&](this auto&& self, int cur, int prv, int64_t par_w) -> int64_t {
				int64_t cur_sum = 0;
				for (auto [nxt, w] : adj[cur]) {
					if (nxt == prv) continue;
					auto ch_val = self(nxt, cur, w);
					ch_val = std::min(ch_val, w);
					cur_sum += ch_val;
					cur_sum = std::min(cur_sum, t);
				}
				int64_t e_top = std::min(t - cur_sum, par_w);
				ans += t - cur_sum - e_top;
				return t - e_top;
			}(0, -1, 0);
			cout << ans << '\n';
		}
	}

	return 0;
}
