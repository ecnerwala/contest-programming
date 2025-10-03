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
					cur_sum += self(nxt, cur, w);
				}
				// if cur_sum >= t, it doesn't matter either, we just don't get anything new
				int64_t e_top = std::max(cur_sum, t - par_w);
				ans += std::max(t, e_top + t - par_w) - t;
				return std::min(e_top, par_w);
			}(0, -1, 0);
			cout << ans << '\n';
		}
	}

	return 0;
}
