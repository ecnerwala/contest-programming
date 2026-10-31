#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v, t; cin >> u >> v >> t;
		adj[u].push_back({v, t});
		adj[v].push_back({u, t});
	}

	int real_fast_time = 0, real_slow_time = 0;
	for (int z = 0; z < 2; z++) {
		std::vector<int> dist(N, -1);
		min_pq<std::pair<int, int>> pq;
		pq.push({dist[0] = 0, 0});
		while (!pq.empty()) {
			auto [d, cur] = pq.top(); pq.pop();
			if (d != dist[cur]) continue;
			for (auto [nxt, l] : adj[cur]) {
				int nd;
				if (z) {
					if (d % 12 + l > 12) {
						nd = d + (12 - d % 12) + l;
					} else {
						nd = d + l;
					}
				} else {
					nd = d + l;
				}
				if (dist[nxt] == -1 || nd < dist[nxt]) {
					pq.push({dist[nxt] = nd, nxt});
				}
			}
		}
		int t = dist[N-1];
		t = (t == 0) ? 0 : ((t-1) / 12 * 24 + (t-1) % 12 + 1);
		(z ? real_slow_time : real_fast_time) = t;
	}

	cout << real_slow_time - real_fast_time << '\n';

	return 0;
}
