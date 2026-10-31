#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		struct edge_t {
			bool is_neg;
			std::array<int, 2> v;
		};
		std::vector<edge_t> edges(M);
		for (auto& e : edges) {
			int o, i, j; std::cin >> o >> i >> j; o--, i--, j--;
			assert(o == 0 || o == 1);
			e.is_neg = o;
			e.v = {i, j};
		}

		std::vector<std::vector<int>> adj(N);
		std::vector<std::array<int, 2>> cnt(N, {0, 0});
		for (int e = 0; e < M; e++) {
			for (int v : edges[e].v) {
				adj[v].push_back(e);
				cnt[v][edges[e].is_neg]++;
			}
		}
		std::vector<int> q; q.reserve(N);
		std::vector<bool> in_queue(N, false);
		for (int v = 0; v < N; v++) {
			if (cnt[v][0] == 0 || cnt[v][1] == 0) {
				q.push_back(v);
				in_queue[v] = true;
			}
		}
		std::vector<int> ans(N, 0);
		for (int z = 0; z < int(q.size()); z++) {
			int v = q[z];
			ans[v] = (N-z) * (cnt[v][0] == 0 ? -1 : +1);
			for (int e : adj[v]) {
				int u = edges[e].v[0] ^ edges[e].v[1] ^ v;
				if (in_queue[u]) continue;
				cnt[u][edges[e].is_neg]--;
				if (cnt[u][0] == 0 || cnt[u][1] == 0) {
					q.push_back(u);
					in_queue[u] = true;
				}
			}
		}
		if (int(q.size()) < N) {
			std::cout << "NO" << '\n';
		} else {
			std::cout << "YES" << '\n';
			for (int i = 0; i < N; i++) {
				assert(ans[i] != 0);
				std::cout << ans[i] << " \n"[i+1==N];
			}
		}
	}

	return 0;
}
