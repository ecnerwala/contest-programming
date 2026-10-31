#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int V, G; cin >> V >> G;

	std::vector<std::vector<std::pair<int, int>>> adj(V);
	for (int g = 0; g < G; g++) {
		int M; cin >> M;
		for (int m = 0; m < M; m++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back({v, g});
			adj[v].push_back({u, g});
		}
	}

	int K; cin >> K;
	std::vector<int> A(K);
	for (auto& a : A) { cin >> a; a--; }

	std::vector<int> next_time(G, K);
	std::vector<int> next_same(K);
	for (int i = K-1; i >= 0; i--) {
		next_same[i] = std::exchange(next_time[A[i]], i);
	}

	std::vector<int> dist(V, K+1);
	std::vector<std::vector<int>> q(K+1);
	q[dist[0] = 0].push_back(0);
	for (int t = 0; t < K; t++) {
		for (int u : q[t]) {
			if (t != dist[u]) continue;
			for (auto [v, g] : adj[u]) {
				int nt = next_time[g]+1;
				if (nt < dist[v]) {
					q[dist[v] = nt].push_back(v);
				}
			}
		}
		next_time[A[t]] = next_same[t];
	}

	int ans = dist[V-1];
	if (ans == K+1) ans = -1;
	cout << ans << '\n';

	return 0;
}
