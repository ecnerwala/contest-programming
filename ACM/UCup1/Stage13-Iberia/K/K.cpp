#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,popcnt,abm,mmx") // Safe for yandex

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::array<int, 2>> edges(M);
	std::vector<std::vector<int>> adj(N);
	{
		std::vector<std::pair<int, int>> deg(N);
		for (auto& [u, v] : edges) {
			cin >> u >> v; u--, v--;
			deg[u].first++, deg[v].first++;
		}

		for (int i = 0; i < N; i++) deg[i].second = i;
		std::sort(deg.begin(), deg.end());

		std::vector<int> renumbering(N);
		for (int i = 0; i < N; i++) {
			renumbering[deg[i].second] = i;
		}

		for (auto& [u, v] : edges) {
			u = renumbering[u], v = renumbering[v];
			if (u > v) std::swap(u, v);
			assert(u < v);
			adj[u].push_back(v);
		}
		for (int i = 0; i < N; i++) {
			std::sort(adj[i].begin(), adj[i].end());
		}
	}

	using bs = bitset<500>;
	std::vector<bs> bss(500);
	std::vector<int> edge_id(N, -1);
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		int K = int(adj[i].size());
		for (int j = 0; j < K; j++) {
			edge_id[adj[i][j]] = j;
		}

		for (int j = K-1; j >= 0; j--) {
			bss[j].reset();
			int m = adj[i][j];
			for (int z = int(adj[m].size())-1; z >= 0; z--) {
				int r = adj[m][z];
				if (edge_id[r] == -1) continue;
				int k = edge_id[r];
				// there's a j-k edge
				ans += (bss[j]&bss[k]).count();
				bss[j].set(k);
			}
		}

		for (int j = 0; j < K; j++) {
			edge_id[adj[i][j]] = -1;
		}
	}
	cout << ans << '\n';

	return 0;
}
