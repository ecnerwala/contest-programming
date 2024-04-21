#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	struct edge_t {
		int dest;
		int cost;
		int reward;
	};
	std::vector<std::vector<edge_t>> adj(N);
	for (int e = 0; e < M; e++) {
		int a, b, c, r; cin >> a >> b >> c >> r; a--, b--;
		adj[a].push_back({b, c, r});
		adj[b].push_back({a, c, r});
	}

	auto is_good = [&](int64_t starting_coins) -> bool {
		std::vector<int64_t> dist(N, -1);
		std::priority_queue<std::pair<int64_t, int>> pq;
		pq.push({dist[0] = starting_coins, 0});
		while (!pq.empty()) {
			auto [d, u] = pq.top(); pq.pop();
			if (d != dist[u]) continue;
			for (auto& e : adj[u]) {
				if (d < e.cost) continue;
				if (e.reward > e.cost) {
					return true;
				}
				if (e.dest == N-1) return true;
				int64_t ndist = d - e.cost + e.reward;
				assert(ndist <= d);
				if (ndist > dist[e.dest]) {
					pq.push({dist[e.dest] = ndist, e.dest});
				}
			}
		}
		return false;
	};
	int64_t mi = -1;
	int64_t ma = 0;
	while (!is_good(ma)) {
		mi = std::exchange(ma, ma * 2);
	}
	while (ma - mi > 1) {
		int64_t md = mi + (ma - mi) / 2;
		if (is_good(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';

	return 0;
}
