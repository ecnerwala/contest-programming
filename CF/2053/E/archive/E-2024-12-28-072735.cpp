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
		cout << [&]() -> int64_t {
			assert(N >= 2);
			if (N == 2) {
				// both initially leaves, so no wins
				return 0;
			}
			// p != q, so there's always one initial vertex
			// if q is intially a leaf, but p is not then Aron win
			int tot_leaves = 0;
			for (int i = 0; i < N; i++) {
				if (int(adj[i].size()) == 1) tot_leaves++;
			}

			int64_t ans = 0;
			ans += int64_t(tot_leaves) * int64_t(N - tot_leaves);

			// Consider (p,q)
			// this is winning for Aron iff q is a leaf XOR all of:
			//  - p is not a leaf and 
			//  - p is not adjacent to a leaf
			//  - y = d(p, q) is adjacent to a leaf
			//  - q is not a leaf

			int num_p = 0;
			int64_t num_q = 0;
			for (int i = 0; i < N; i++) {
				// don't care about leaves;
				if (int(adj[i].size()) == 1) continue;
				int num_leaves = 0;
				for (int j : adj[i]) {
					if (int(adj[j].size()) == 1) num_leaves++;
				}
				if (num_leaves == 0) {
					num_p++;
				} else {
					num_q += std::max(int(adj[i].size()) - num_leaves - 1, 0);
				}
			}
			ans += int64_t(num_p) * int64_t(num_q);

			return ans;
		}() << '\n';
	}

	return 0;
}
