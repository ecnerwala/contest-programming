#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		if (N <= 2) {
			cout << 1 << '\n';
			for (int i = 0; i < N; i++) cout << i << '\n';
			continue;
		}
		std::vector<int> sz(N);
		[&](this auto&& self, int cur, int prv) -> void {
			sz[cur] = 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				sz[cur] += sz[nxt];
			}
		}(0, -1);
		int centroid = 0;
		while (true) {
			int nxt = -1;
			for (int ch : adj[centroid]) {
				if (sz[ch] >= sz[centroid]) continue;
				if (sz[ch] * 2 >= N) {
					nxt = ch;
					break;
				}
			}
			if (nxt == -1) break;
			centroid = nxt;
		}
		assert(int(adj[centroid].size()) >= 2);
		int num_leaves = 0;
		std::vector<int> any_leaf(N);
		[&](this auto&& self, int cur, int prv) -> void {
			if (adj[cur].size() == 1) {
				assert(prv != -1);
				any_leaf[cur] = num_leaves;
				num_leaves++;
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				any_leaf[cur] = any_leaf[nxt];
			}
		}(centroid, -1);
		int K = (num_leaves + 1) / 2;
		std::vector<std::vector<int>> pos(N);
		pos[centroid] = std::vector<int>(K, 0);
		[&](this auto&& self, int cur, int prv) -> void {
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				pos[nxt] = pos[cur];
				if (any_leaf[nxt] >= K) pos[nxt][any_leaf[nxt] - K]--;
				else pos[nxt][any_leaf[nxt]]++;
				self(nxt, cur);
			}
		}(centroid, -1);
		cout << K << '\n';
		for (auto& p : pos) {
			for (int i = 0; i < K; i++) cout << p[i] << " \n"[i+1==K];
		}
	}

	return 0;
}
