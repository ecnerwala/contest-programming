#include <bits/stdc++.h>
template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N, M, H; std::cin >> N >> M >> H;
		std::vector<int> horses(H);
		for (auto& a : horses) { std::cin >> a; a--; }
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v, w; std::cin >> u >> v >> w; u--, v--;
			assert(!(w & 1)); w /= 2;
			adj[u].push_back({v, w});
			adj[v].push_back({u, w});
		}
		auto dijk = [&](int st) -> std::vector<int64_t> {
			std::vector<int64_t> dist(N, -1);
			min_pq<std::pair<int64_t, int>> pq;
			pq.push({dist[st] = 0, st});
			while (!pq.empty()) {
				auto [d, cur] = pq.top(); pq.pop();
				if (d != dist[cur]) continue;
				for (auto [nxt, l] : adj[cur]) {
					int64_t nd = d + l;
					if (dist[nxt] == -1 || nd < dist[nxt]) {
						pq.push({dist[nxt] = nd, nxt});
					}
				}
			}
			return dist;
		};
		auto d0 = dijk(0);
		auto d1 = dijk(N-1);
		if (d0[N-1] == -1) {
			std::cout << -1 << '\n';
		} else {
			int64_t ans = 2 * d0[N-1];
			for (auto h : horses) {
				if (d0[h] == -1) continue;
				ans = std::min(ans, d0[h] + d1[h] + std::min(d0[h], d1[h]));
			}
			std::cout << ans << '\n';
		}
	}
}
