#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;

		std::vector<int> exits(K);
		for (auto& e : exits) { cin >> e; e--; }

		std::vector<int> D(N);
		for (auto& d : D) { cin >> d; }

		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v, w; cin >> u >> v >> w; u--, v--;
			adj[u].push_back({v, w});
			adj[v].push_back({u, w});
		}

		std::vector<int> dist(N, -1);
		min_pq<std::pair<int, int>> pq;
		for (int e : exits) {
			D[e] = 0;
			pq.push({0, e});
		}
		while (!pq.empty()) {
			auto [d, cur] = pq.top(); pq.pop();
			if ((D[cur] --) != 0) continue;
			dist[cur] = d;
			for (auto [nxt, l] : adj[cur]) {
				pq.push({d + l, nxt});
			}
		}
		cout << dist[0] << '\n';
	}

	return 0;
}
