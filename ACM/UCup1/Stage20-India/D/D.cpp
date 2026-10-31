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

		std::vector<int> q; q.reserve(N);
		std::vector<int> dist(N, -1);
		dist[0] = 0;
		q.push_back(0);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt : adj[cur]) {
				if (dist[nxt] == -1) {
					dist[nxt] = dist[cur] + 1;
					q.push_back(nxt);
				}
			}
		}

		assert(int(q.size()) == N);

		int RT = 0;
		while (RT * RT < N) RT++;

		// (N-1) // RT+1 + 1 == (N+RT) // RT+1
		std::vector<int> cnd(RT+1, 1);
		for (int v = 1; v < N; v++) {
			assert(dist[v] >= 1);
			cnd[dist[v] % (RT+1)]++;
		}

		int take = int(std::min_element(cnd.begin(), cnd.end()) - cnd.begin());

		std::vector<int> res; res.reserve(cnd[take]);
		res.push_back(0);
		for (int v = 1; v < N; v++) {
			assert(dist[v] >= 1);
			if (dist[v] % (RT+1) == take) {
				res.push_back(v);
			}
		}
		cout << res.size() << '\n';
		for (int i = 0; i < int(res.size()); i++) {
			cout << res[i]+1 << " \n"[i+1==int(res.size())];
		}
	}

	return 0;
}
