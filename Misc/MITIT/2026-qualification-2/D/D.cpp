#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;

		struct edge_t {
			int a, b, s;
		};
		std::vector<edge_t> edges(M);
		for (auto& e : edges) {
			std::cin >> e.a >> e.b >> e.s; e.a--, e.b--, e.s--;
			if (e.s == -2) e.s = -1;
		}

		std::vector<int> in_deg(N);
		std::vector<std::vector<int>> bads(N);
		for (int e = 0; e < M; e++) {
			if (edges[e].s != -1) {
				int c = edges[edges[e].s].b;
				in_deg[edges[e].b]++;
				bads[c].push_back(edges[e].b);
			} else {
				assert(edges[e].b == N-1);
			}
		}
		assert(in_deg[N-1] == 0);
		std::vector<int> q; q.reserve(N);
		for (int i = 0; i < N; i++) if (in_deg[i] == 0) q.push_back(i);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int o : bads[cur]) {
				if (!(--in_deg[o])) {
					q.push_back(o);
				}
			}
		}
		std::cout << (int(q.size()) < N ? "YES" : "NO") << '\n';
	}

	return 0;
}
