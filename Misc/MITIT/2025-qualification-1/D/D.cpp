#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		std::vector<int> X(K); for (auto& x : X) { cin >> x; x--; }

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<std::array<int, 2>> extras; extras.reserve(K);
		for (int i = 0; i < K; i++) {
			int u = (i ? X[i-1] : 0);
			int v = X[i];
			if (u != v) extras.push_back({u, v});
		}
		for (auto [u, v] : extras) {
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> dist(N, -1);
		std::vector<int> q; q.reserve(N);
		dist[0] = 0;
		q.push_back(0);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt : adj[cur]) {
				if (dist[nxt] == -1) {
					dist[nxt] = dist[cur] + 1;
					q.push_back(nxt);
				}
			}
		}
		assert(int(q.size()) == N);
		bool good = true;
		for (int i = 0; i+1 < K; i++) {
			good = good && dist[X[i]] < dist[X[i+1]];
		}
		if (good) {
			cout << extras.size() << '\n';
			for (auto [u, v] : extras) {
				cout << u+1 << ' ' << v+1 << '\n';
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
