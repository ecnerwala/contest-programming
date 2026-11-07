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
		std::cout << [&]() -> int {
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
				if (indeg[i] == 0) {
					return -1;
				}
			}
			for (auto v : cnt) { std::cerr << v << ' '; } std::cerr << '\n';
			return int(q.size());
		}() << '\n';
	}

	return 0;
}
