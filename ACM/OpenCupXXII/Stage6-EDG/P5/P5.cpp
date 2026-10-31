#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, C; cin >> N >> M >> C;

	std::vector<std::vector<std::pair<int, int>>> adj(N);
	bool has_edge = false;
	for (int e = 0; e < M; e++) {
		int u, v, p; cin >> u >> v >> p; u--, v--;
		if (p > C) {
			continue;
		}
		has_edge = true;
		adj[u].push_back({v, p});
	}

	if (!has_edge) {
		cout << 0 << '\n';
		exit(0);
	}

	std::vector<int> dist(N);
	min_pq<pair<int, int>> pq;
	for (int i = 0; i < N; i++) {
		std::fill(dist.begin(), dist.end(), C+1);
		pq.push({dist[i] = 0, i});
		while (!pq.empty()) {
			auto [d, cur] = pq.top(); pq.pop();
			if (d != dist[cur]) continue;
			for (auto [nxt, l] : adj[cur]) {
				int nd = l + d;
				if (nd > C) continue;
				if (nxt == i) {
					cout << 2 << '\n';
					exit(0);
				}
				if (nd < dist[nxt]) {
					pq.push({dist[nxt] = nd, nxt});
				}
			}
		}
	}

	cout << 1 << '\n';

	return 0;
}
