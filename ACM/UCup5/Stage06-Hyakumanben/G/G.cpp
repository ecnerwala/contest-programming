#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::vector<std::vector<int>> edges(N);
		std::vector<int> indeg(N, 0);
		for (int e = 0; e < M; e++) {
			int u, v; std::cin >> u >> v; u--, v--;
			edges[u].push_back(v);
			edges[v].push_back(u);
			indeg[v]++;
		}
		std::cout << [&]() -> int {
			std::vector<int> q; q.reserve(N+1);
			std::vector<int> cnt(N, 0);
			for (int i = 0; i < N; i++) {
				if (!indeg[i]) {
					cnt[i]++;
					q.push_back(i);
				}
			}
			for (int z = 0; z < int(q.size()); z++) {
				int i = q[z];
				assert(indeg[i] == 0);
				for (int j : edges[i]) {
					assert(j != i);
					indeg[i]++;
					indeg[j]--;
					if (indeg[j] == 0) {
						if (cnt[j]) return -1;
						cnt[j]++;
						q.push_back(j);
					}
				}
			}
			return int(q.size());
		}() << '\n';
	}

	return 0;
}
