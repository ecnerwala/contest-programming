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
		std::vector<int> pref(N+1);
		[&](this auto&& self, int cur, int prv, int d) -> int {
			int max_depth = d;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				max_depth = std::max(max_depth, self(nxt, cur, d+1));
			}
			// keep this guy in [depth, max_depth]
			pref[d]++;
			pref[max_depth+1]--;
			return max_depth;
		}(0, -1, 0);
		for (int i = 0; i < N; i++) pref[i+1] += pref[i];
		cout << N - *std::max_element(pref.begin(), pref.end()) << '\n';
	}

	return 0;
}
