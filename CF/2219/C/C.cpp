#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string S; std::cin >> S;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; std::cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		int rt = 0;
		while (S[rt] != '1') rt++;

		using dbl = double;
		const dbl INF = 1e99;
		[&](this auto&& self, int cur, int prv) -> std::array<dbl, 2> {
			dbl base = 0;
			std::vector<dbl> ch_vals; ch_vals.reserve(adj[cur].size());
			for (int nxt : adj[cur]) {
				auto ch_val = self(nxt, cur);
				ch_vals.push_back(ch_val[1] - ch_val[0]);
				base += ch_val[0];
			}
			if (S[cur] == '1') {
				return {base, base};
			}
			std::sort(ch_vals.begin(), ch_vals.end());
			std::array<dbl, 2> best_cost{INF, INF};
			int deg = int(adj[cur].size());
			for (int cnt = 0; cnt <= int(ch_vals.size()); cnt++) {
				if (cnt > 0) base += ch_vals[cnt-1];
				if (cnt > 0) {
					best_cost[1] = std::min(best_cost[1], base + dbl(deg) / cnt);
				}
				best_cost[0] = std::min(best_cost[0], base + dbl(deg) / (cnt + 1));
			}
			return best_cost;
		}(rt, -1);
	}

	return 0;
}
