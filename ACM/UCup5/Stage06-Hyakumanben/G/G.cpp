#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		std::vector<std::vector<int>> outedges(N);
		std::vector<int> indeg(N, 0);
		for (int e = 0; e < M; e++) {
			int u, v; std:::cin >> u >> v; u--, v--;
		}
	}

	return 0;
}
