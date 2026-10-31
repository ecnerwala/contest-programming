#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> deg(N);
	std::vector<std::pair<int, std::array<int, 2>>> edges(N-1);
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v, c; cin >> u >> v >> c; u--, v--;
		deg[u]++, deg[v]++;
		edges[e] = {c, {u, v}};
		adj[u].push_back({v, e});
		adj[v].push_back({u, e});
	}
	[&](this auto&& self, int cur, int prv) -> bool {
		bool sz = 0;
		if (int(adj[cur].size()) - (prv != -1) == 0) {
			// we're a leaf
			return true;
		}
		for (auto [nxt, e] : adj[cur]) {
			if (nxt == prv) continue;
			bool ch = self(nxt, cur);
			if ((edges[e].first & 1) != ch) edges[e].first--;
			assert((edges[e].first & 1) == ch);
			sz ^= ch;
		}
		return sz;
	}(0, -1);
	for (auto e : edges) {
		if (e.first < 0) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	std::vector<int> cur_deg = deg;
	std::vector<bool> alive(N-1, true);
	min_pq<std::pair<int, int>> ops;
	for (int e = 0; e < N-1; e++) {
		ops.push({edges[e].first, e});
	}
	for (int i = 0; i < N; i++) {
		if (deg[i] == 1) {
			// leaf
			ops.push({1, adj[i][0].second});
		}
	}
	while (!ops.empty()) {
		auto [w, e] = ops.top(); ops.pop();
		if (!alive[e]) continue;
		alive[e] = false;
		edges[e].first = w;
		for (auto v : edges[e].second) {
			cur_deg[v]--;
			if (cur_deg[v] == 1) {
				int vw = 0;
				for (auto [_, f] : adj[v]) {
					if (!alive[f]) vw += edges[f].first;
				}
				for (auto [_, f] : adj[v]) {
					if (alive[f]) ops.push({vw, f});
				}
			}
		}
	}
	int64_t ans = 0;
	for (int e = 0; e < N-1; e++) {
		ans += edges[e].first;
	}
	cout << ans << '\n';

	return 0;
}
