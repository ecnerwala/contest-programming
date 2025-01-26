#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> bounds(N);
		for (auto& [l, r] : bounds) cin >> l >> r;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		int64_t ans = 0;
		ans += [&](this auto&& self, int cur, int prv) -> int {
			auto [v, r] = bounds[cur];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				int nv = self(nxt, cur);
				v = std::min(nv = std::max(nv, v), r);
				ans += nv - v;
			}
			return v;
		}(0, -1);
		cout << ans << '\n';
	}

	return 0;
}
