#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, R; cin >> N >> R;
	vector<int> T(N); for (auto& t : T) { cin >> t; t--; }

	vector<vector<pair<int, int64_t>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int a, b, c; cin >> a >> b >> c; a--, b--;
		adj[a].push_back({b, c});
		adj[b].push_back({a, c});
	}

	int Q; cin >> Q;
	vector<vector<pair<int, int>>> point_queries(N);
	for (int q = 0; q < Q; q++) {
		int u, v, r; cin >> u >> v >> r; u--, v--, r--;
		point_queries[u].push_back({r, q});
		point_queries[v].push_back({r, q});
	}

	return 0;
}
