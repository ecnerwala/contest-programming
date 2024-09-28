#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		std::vector<std::vector<std::tuple<int, int, int>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v, c, l; cin >> u >> v >> c >> l;
			u--, v--, c--;
			adj[u].push_back({v, c, l});
			adj[v].push_back({u, c, l});
		}
		std::vector<std::pair<int, int>> tickets(K);
		for (auto& [a, b] : tickets) { cin >> a >> b; a--; }

		using dist_t = std::pair<int, int>;
		const dist_t INF = {K+1, 0};
		std::vector<min_pq<std::pair<int, int>>> company_dist(M);
		std::vector<dist_t> dist(N, INF);
		// K >= 1
		company_dist[tickets[0].first].push({0, 0});

		for (int k = 0; k < K; k++) {
			auto [ticket_comp, ticket_len] = tickets[k];
			min_pq<std::pair<int, int>> pq;
			{
				auto& init_q = company_dist[ticket_comp];
				while (!init_q.empty() && init_q.top().first <= ticket_len) {
					auto [d, v] = init_q.top();
					init_q.pop();
					dist_t cnd = {k, d};
					if (cnd < dist[v]) pq.push({(dist[v] = cnd).second, v});
				}
			}
			while (!pq.empty()) {
				auto [d, v] = pq.top();
				pq.pop();
				assert(d == k);
				assert(dist[v].first == k);
				assert(dist[v].second >= d);
				if (dist[v].second != d) continue;
				for (auto [u, c, l] : adj[v]) {
					if (c == ticket_comp && l + d <= ticket_len) {
						dist_t cnd{k, d + l};
						if (cnd < dist[u]) pq.push({(dist[u] = cnd).second, u});
					} else {
						company_dist[c].push({l, u});
					}
				}
			}
		}
		for (int i = 0; i < N; i++) {
			cout << int(dist[i] != INF);
		}
		cout << '\n';
	}

	return 0;
}
