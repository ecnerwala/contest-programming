#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M, K; std::cin >> N >> M >> K;
		assert(N >= 4);
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) { std::cin >> u >> v; u--, v--; }

		std::cout << ([&]() -> bool {
			// Total edge parity doesn't matter, since we can swap vertices 0 and 2 to flip it

			if (!(K & 1)) return true;

			std::vector<std::vector<int>> adj(N);

			int tgt_1s = 0;
			std::vector<int> out_deg(N);
			for (int i = 0; i < N; i++) {
				out_deg[i] = (N-1-i) & 1;
				tgt_1s += out_deg[i];
			}
			for (auto [u, v] : edges) {
				adj[u].push_back(v);
				adj[v].push_back(u);
				if (u > v) {
					out_deg[u] ^= 1;
					out_deg[v] ^= 1;
				}
			}

			for (auto& e : adj) std::sort(e.begin(), e.end());

			// Now, see if we can flip enough guys
			std::vector<bool> visited(N, false);
			std::set<int> unvisited;
			for (int i = 0; i < N; i++) unvisited.insert(i);

			int mi_1 = 0;
			int ma_1 = 0;
			for (int i = 0; i < N; i++) {
				if (visited[i]) continue;
				std::array<int, 2> cnt{0, 0};
				[&](this auto&& self, int cur) -> void {
					assert(!visited[cur]);
					visited[cur] = true;
					unvisited.erase(cur);
					cnt[out_deg[cur]]++;
					int nxt = 0;
					auto adj_it = adj[cur].begin();
					while (true) {
						auto it = unvisited.lower_bound(nxt);
						if (it == unvisited.end()) break;
						nxt = *it;
						while (adj_it != adj[cur].end() && *adj_it < nxt) ++*adj_it;
						if (adj_it != adj[cur].end() && *adj_it == nxt) {
							// skip it
						} else {
							self(nxt);
						}
						nxt++;
					}
				}(i);
				int tot = cnt[0] + cnt[1];
				int mi = 0, ma = tot;
				if ((mi & 1) != (cnt[1] & 1)) mi++;
				if ((ma & 1) != (cnt[1] & 1)) ma--;
				mi_1 += mi;
				ma_1 += ma;
			}
			assert((mi_1 & 1) == (tgt_1s & 1));
			assert((ma_1 & 1) == (tgt_1s & 1));
			return (mi_1 <= tgt_1s && tgt_1s <= ma_1);
		}() ? "Yes" : "No") << '\n';
	}

	return 0;
}
