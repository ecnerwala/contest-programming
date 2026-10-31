#include <bits/stdc++.h>
template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N, M, H; std::cin >> N >> M >> H;
		std::vector<int> horses(H);
		for (auto& a : horses) { std::cin >> a; a--; }
		std::vector<std::vector<std::pair<int, int>>> adj(2*N);
		for (int e = 0; e < M; e++) {
			int u, v, w; std::cin >> u >> v >> w; u--, v--;
			adj[u].push_back({v, w});
			adj[v].push_back({u, w});
			assert(!(w & 1)); w /= 2;
			adj[N+u].push_back({N+v, w});
			adj[N+v].push_back({N+u, w});
		}
		for (auto a : horses) {
			adj[a].push_back({N+a, 0});
		}
		auto dijk = [&](int st) -> std::vector<int64_t> {
			std::vector<int64_t> dist(2*N, -1);
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
			if (d0[N] != -1) {
				for (int i = 0; i < N; i++) {
					if (d0[i] == -1) continue;
					assert(d0[i] != -1 && d1[i] != -1);
					assert(d0[N+i] != -1);
					ans = std::min(ans, std::max(std::min(d0[i], d0[N+i]), std::min(d1[i], d1[N+i])));
				}
			}
			std::cout << ans << '\n';
		}
	}
}
