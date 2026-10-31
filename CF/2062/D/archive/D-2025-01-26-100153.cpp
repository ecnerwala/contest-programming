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

		std::vector<int> val(N);
		int64_t ans = 0;
		[&](this auto&& self, int cur, int prv) -> void {
			int v = bounds[cur][0];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				v = std::max(v, val[nxt]);
			}
			v = std::min(v, bounds[cur][1]);
			val[cur] = v;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				ans += std::max(val[nxt] - val[cur], 0);
			}
		}(0, -1);
		ans += val[0];
		cout << ans << '\n';
	}

	return 0;
}
