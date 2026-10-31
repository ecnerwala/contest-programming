#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K, V; std::cin >> N >> K >> V; V--;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; std::cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		assert(int(adj[V].size()) >= 2);
		// min dist to a forbidden node
		int d_root = [&](this auto&& self, int cur, int prv) -> int {
			if (int(adj[cur].size()) == 1) {
				// we're a leaf
				assert(prv != -1);
				// min dist to a leaf
				return 0;
			}
			int d = K+2;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				int nd = self(nxt, cur) + 1;
				if (d + nd <= K+1) return 0;
				d = std::min(d, nd);
			}
			return d;
		}(V, -1);
		if (d_root == 0) {
			std::cout << "YES" << '\n';
		} else {
			std::cout << "NO" << '\n';
		}
	}

	return 0;
}

// We need there to be a region of size
