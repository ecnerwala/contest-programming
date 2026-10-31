#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	vector<int> colors(N);
	for (auto& c : colors) cin >> c, c--;
	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<vector<int64_t>> dp(1<<K, vector<int64_t>(N, 0));

	for (int i = 0; i < N; i++) {
		dp[1 << colors[i]][i] ++;
	}

	int64_t ans = 0;
	for (int msk = 1; msk < (1 << K); msk++) {
		for (int i = 0; i < N; i++) {
			int64_t v = dp[msk][i];
			if (!v) continue;
			ans += v;
			for (int j : adj[i]) {
				if (msk & (1 << colors[j])) continue;
				dp[msk | (1 << colors[j])][j] += v;
			}
		}
	}
	ans -= N;
	cout << ans << '\n';

	return 0;
}
