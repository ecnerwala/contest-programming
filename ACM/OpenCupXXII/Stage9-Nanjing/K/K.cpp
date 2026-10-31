#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	std::vector<std::array<int, 2>> edges(M);
	for (auto& e : edges) { cin >> e[0] >> e[1]; e[0]--, e[1]--; }

	std::vector<std::vector<int>> adj(N);
	std::vector<std::vector<std::pair<int, int>>> big_adj(N);

	std::vector<std::pair<int, int>> deg(N);
	for (int i = 0; i < N; i++) deg[i].second = i;
	for (auto [x, y] : edges) {
		deg[x].first++;
		deg[y].first++;
	}
	for (int e = 0; e < M; e++) {
		auto [x, y] = edges[e];
		adj[x].push_back(y);
		adj[y].push_back(x);
		if (deg[y] > deg[x]) std::swap(x, y);
		big_adj[y].push_back({x, e});
	}

	int64_t num_none = int64_t(__int128(N) * __int128(N-1) * __int128(N-2) * __int128(N-3) / 24);
	int64_t num_edge = int64_t(M) * int64_t(N-2) * int64_t(N-3) / 2;

	int64_t num_pair = int64_t(M) * int64_t(M-1) / 2;
	int64_t num_2path = 0;
	for (int i = 0; i < N; i++) {
		int x = deg[i].first;
		num_2path += int64_t(x) * int64_t(x-1) / 2;
	}
	num_pair -= num_2path;
	num_2path *= int64_t(N-2);

	int64_t ans = num_none - num_edge + num_pair + num_2path;
	return 0;
}
