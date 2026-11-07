#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::vector<std::vector<int>> outedges(N);
		std::vector<std::vector<int>> inedges(N);
		std::vector<int> indeg(N, 0);
		for (int e = 0; e < M; e++) {
			int u, v; std::cin >> u >> v; u--, v--;
			outedges[u].push_back(v);
			inedges[v].push_back(u);
			indeg[v]++;
		}
		std::cout << [&]() -> int64_t {
			{
				std::vector<int> q; q.reserve(N+1);
				for (int i = 0; i < N; i++) {
					if (!indeg[i]) {
						q.push_back(i);
					}
				}
				for (int z = 0; z < int(q.size()); z++) {
					int i = q[z];
					assert(indeg[i] == 0);
					for (int j : outedges[i]) {
						assert(j != i);
						indeg[j]--;
						assert(indeg[j] >= 0);
						if (indeg[j] == 0) {
							q.push_back(j);
						}
					}
				}
			}
			// Now, everything with nonzero indeg is a source
			std::vector<int> dist(N, -1);
			std::vector<int> q0; q0.reserve(N);
			std::vector<int> q1; q1.reserve(N);
			for (int i = 0; i < N; i++) if (indeg[i]) {
				dist[i] = 0;
				q0.push_back(i);
			}
			return std::accumulate(dist.begin(), dist.end(), int64_t(0));
		}() << '\n';
	}

	return 0;
}
