#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<std::vector<std::pair<int, int>>> by_depth(N);
		[&](this auto&& self, int cur, int prv, int depth) -> void {
			by_depth[depth].push_back({cur, prv});
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur, depth+1);
			}
		}(0, -1, 0);
		int ans = 0;
		for (int d = 0; d < N; d++) {
			if (by_depth[d].empty()) break;
			if (d > 0 && [&]() -> bool {
				int p = by_depth[d][0].second;
				for (auto [_, op] : by_depth[d]) if (op != p) return false;
				return true;
			}()) {
				ans = std::max(ans, int(by_depth[d].size())+1);
			} else {
				ans = std::max(ans, int(by_depth[d].size()));
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
