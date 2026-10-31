#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N, M; std::cin >> N >> M;
	std::vector<std::array<int, 2>> edges(M);
	for (auto& [u, v] : edges) { std::cin >> u >> v; u--, v--; }

	std::vector<int> S(N); for (auto& s : S) { std::cin >> s; s--; }

	std::vector<int> loc(N); for (int i = 0; i < N; i++) loc[S[i]] = i;

	std::vector<std::vector<int>> adj(N);
	for (auto [u, v] : edges) {
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	for (auto& r : adj) {
	}

	std::vector<int> vis(N);
	std::vector<int> S_real; S_real.reserve(N);
	[&](this auto&& self, int cur) -> void {
		if (vis[cur]) return;
		vis[cur] = true;
		S_real.push_back(cur);
		std::sort(adj[cur].begin(), adj[cur].end(), [&](int a, int b) -> bool { return loc[a] < loc[b]; });
		for (auto nxt : adj[cur]) { self(nxt); }
	}(0);
	if (S_real != S) {
		std::cout << "impossible" << '\n';
	} else {
		for (int j = 0; j < M; j++) {
			auto [u, v] = edges[j];
			std::cout << std::max(loc[u], loc[v]) + 1 << " \n"[j+1==M];
		}
	}


	return 0;
}
