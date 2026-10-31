#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K; cin >> N >> K;
	vector<int> deg(N);
	vector<vector<int>> nei(N);
	vector<vector<bool>> adj(N, vector<bool>(N, false));
	for (int i = 0; i < N; i++) {
		int d; cin >> d;
		deg[i] = d;
		nei[i].reserve(d);
		while (d--) {
			int v; cin >> v;
			nei[i].push_back(v);
			adj[i][v] = true;
		}
	}

	vector<bool> alive(N, true);

	vector<int> sparses; sparses.reserve(N);
	for (int i = 0; i < N; i++) {
		if (deg[i] < K) sparses.push_back(i);
	}

	int ans = 0;
	vector<int64_t> dp; dp.reserve(1 << (K-1));
	for (int z = 0; z < int(sparses.size()); z++) {
		int cur = sparses[z];
		assert(deg[cur] < K);
		assert(alive[cur]);
		alive[cur] = false;
		vector<int> cnds; cnds.reserve(deg[cur]);
		for (int nxt : nei[cur]) {
			if (!alive[nxt]) continue;
			cnds.push_back(nxt);
			if (deg[nxt]-- == K) {
				sparses.push_back(nxt);
			}
		}
		assert(int(cnds.size()) == deg[cur]);
		assert(int(cnds.size()) < K);
		int M = int(cnds.size());
		dp.resize(1<<M);
		dp[0] = 0;
		for (int i = 0; i < M; i++) {
			int16_t v = 0;
			for (int j = 0; j < M; j++) {
				if (i != j && !adj[cnds[i]][cnds[j]]) v |= (1 << j);
			}
			for (int m = 0; m < (1 << i); m++) {
				dp[(1<<i)+m] = dp[m] | v;
			}
		}
		for (int m = 0; m < (1 << M); m++) {
			if (dp[m] & m) {
				continue;
			}
			ans = max(ans, 1 + __builtin_popcount(m));
		}
	}
	assert(int(sparses.size()) == N);

	cout << ans << '\n';

	return 0;
}
