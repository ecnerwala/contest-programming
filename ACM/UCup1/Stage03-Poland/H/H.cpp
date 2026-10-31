#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Z; cin >> Z;
	while (Z--) {
		int N, M; cin >> N >> M;
		std::vector<std::pair<std::array<int, 2>, int>> edges(M);
		for (auto& [vs, d] : edges) {
			cin >> vs[0] >> vs[1] >> d; vs[0]--, vs[1]--;
		}

		{
			std::vector<std::vector<std::pair<int, int>>> adj(N);
			for (int e = 0; e < M; e++) {
				auto [u, v] = edges[e].first;
				auto d = edges[e].second;
				adj[u].push_back({v, d});
				adj[v].push_back({u, d});
			}
			std::array<std::vector<int64_t>, 2> dist;
			std::vector<int> by_dist; by_dist.reserve(N);
			for (int z = 0; z < 2; z++) {
				dist[z].assign(N, -1);
				min_pq<std::pair<int64_t, int>> pq;
				int st = z == 0 ? 0 : N - 1;
				pq.push({dist[z][st] = 0, st});
				while (!pq.empty()) {
					auto [d, cur] = pq.top(); pq.pop();
					if (dist[z][cur] != d) continue;
					if (z == 0) by_dist.push_back(cur);
					for (auto [nxt, l] : adj[cur]) {
						int64_t nd = d + l;
						if (dist[z][nxt] == -1 || nd < dist[z][nxt]) {
							pq.push({dist[z][nxt] = nd, nxt});
						}
					}
				}
			}

			int64_t tot_len = dist[0][N-1];
			edges.clear();
			for (int cur : by_dist) {
				for (auto [nxt, l] : adj[cur]) {
					if (dist[0][cur] + l + dist[1][nxt] == tot_len) {
						edges.push_back({{cur, nxt}, l});
					}
				}
			}
		}

		std::vector<std::pair<std::array<int, 2>, int>> n_edges; n_edges.reserve(M);
		std::vector<int> dist_forwards(N);
		std::vector<int> dist_backwards(N);

		int prv_val = -1;
		while (true) {
			int cur_val = -1;
			for (auto [_, l] : edges) {
				if (prv_val == -1 || l < prv_val) {
					if (cur_val == -1 || l > cur_val) cur_val = l;
				}
			}
			if (cur_val == -1) {
				break;
			}

			std::fill(dist_forwards.begin(), dist_forwards.end(), -1);
			dist_forwards[0] = 0;
			for (auto [vs, l] : edges) {
				auto [u, v] = vs;
				if (dist_forwards[u] == -1) continue;
				dist_forwards[v] = std::max(dist_forwards[v], dist_forwards[u] + (l == cur_val));
			}
			std::fill(dist_backwards.begin(), dist_backwards.end(), -1);
			dist_backwards[N-1] = 0;
			std::reverse(edges.begin(), edges.end());
			for (auto [vs, l] : edges) {
				auto [u, v] = vs;
				if (dist_backwards[v] == -1) continue;
				dist_backwards[u] = std::max(dist_backwards[u], dist_backwards[v] + (l == cur_val));
			}
			std::reverse(edges.begin(), edges.end());

			int tgt = dist_forwards[N-1];

			n_edges.clear();
			for (auto [vs, l] : edges) {
				auto [u, v] = vs;
				if (dist_forwards[u] + (l == cur_val) + dist_backwards[v] == tgt) {
					n_edges.push_back({{u, v}, l});
				}
			}
			std::swap(edges, n_edges);

			prv_val = cur_val;
		}

		std::vector<int> path; path.reserve(N);
		path.push_back(0);
		for (auto [vs, l] : edges) {
			auto [u, v] = vs;
			if (u == path.back()) {
				path.push_back(v);
			}
		}
		assert(path.back() == N-1);
		cout << path.size() << '\n';
		for (int i = 0; i < int(path.size()); i++) {
			cout << path[i]+1 << " \n"[i+1==int(path.size())];
		}
	}

	return 0;
}
