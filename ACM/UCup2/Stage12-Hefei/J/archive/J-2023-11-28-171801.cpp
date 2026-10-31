#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v, w; cin >> u >> v >> w;
		u--, v--;
		adj[u].push_back({v, w});
		adj[v].push_back({u, w});
	}

	std::array<std::vector<int>, 2> dist;
	for (int z = 0; z < 2; z++) {
		dist[z].assign(N, -1);
		std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
		int st = z ? N-1 : 0;
		pq.push({dist[z][st] = 0, st});
		while (!pq.empty()) {
			auto [d, cur] = pq.top(); pq.pop();
			if (d != dist[z][cur]) continue;
			for (auto [nxt, w] : adj[cur]) {
				int cnd = std::min(d, w);
				if (dist[z][nxt] == -1 || cnd < dist[z][nxt]) {
					pq.push({dist[z][nxt] = cnd, nxt});
				}
			}
		}
	}

	int64_t ans = 1e18;
	for (int i = 0; i < N; i++) {
		for (auto [j, w] : adj[i]) {
			int v = std::max(dist[0][i], dist[1][j]);
			if (v <= w) {
				cerr << i << ' ' << j << ' ' << w << ' ' << v << '\n';
				ans = std::min<int64_t>(ans, int64_t(v) + int64_t(w));
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
