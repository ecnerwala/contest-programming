#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<std::pair<int, int>> verts(N);
		for (int i = 0; i < N; i++) {
			verts[i] = {int(adj[i].size()), i};
		}
		std::sort(verts.begin(), verts.end());
		std::vector<int> idx(N);
		for (int i = 0; i < N; i++) idx[verts[i].second] = i;

		std::vector<std::vector<int>> adj_earlier;
		for (int i = 0; i < N; i++) {
			adj_earlier[i].reserve(adj[i].size());
			for (auto v : adj[i]) {
				if (idx[v] < idx[i]) adj_earlier[i].push_back(v);
			}
		}

		std::vector<bool> alive(N, false);

		std::vector<std::pair<int, std::vector<int>>> res; res.reserve(N);
		bool bad = false;

		std::vector<int> cnd; cnd.reserve(N);
		for (auto [_, i] : verts) {
			res.emplace_back();
			res.back().first = i;
			res.back().second.reserve(adj_earlier[i].size());

			cnd.clear();
			for (int j : adj_earlier[i]) {
				if (!alive[j]) continue;
				res.back().second.push_back(j);
				alive[j] = false;
				cnd.insert(cnd.end(), adj_earlier[j].begin(), adj_earlier[j].end());
			}
			assert(!alive[i]);
			alive[i] = true;
			std::sort(cnd.begin(), cnd.end());
			std::sort(adj_earlier[i].begin(), adj_earlier[i].end());
			if (adj_earlier[i] != cnd) {
				bad = true;
				break;
			}
		}
		if (bad) {
			cout << "No" << '\n';
		} else {
			cout << "Yes" << '\n';
			for (int i = 0; i < N; i++) {
				cout << res[i].first+1 << ' ' << res[i].second.size();
				for (auto v : res[i].second) cout << ' ' << v;
				cout << '\n';
			}
		}
	}

	return 0;
}
