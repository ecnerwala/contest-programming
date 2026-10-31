#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<uint32_t> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v;
		u--, v--;
		adj[u] |= uint32_t(1) << v;
		adj[v] |= uint32_t(1) << u;
	}

	vector<uint32_t> all_adj(1 << N);
	for (int i = 0; i < N; i++) {
		for (int m = 0; m < (1 << i); m++) {
			all_adj[(1 << i) + m] = all_adj[m] | adj[i];
		}
	}

	vector<int64_t> dp(1 << N);
	dp[0] = 1;
	for (int m = 0; m < (1 << N); m++) {
		for (int v = m; v > 0; v = (v-1) & m) {
			if (v & all_adj[v]) continue;
			if (((1<<N)-1-m) & ~all_adj[v]) continue;
			dp[m] += dp[m ^ v];
		}
	}
	cout << dp[(1<<N)-1] << '\n';

	return 0;
}
