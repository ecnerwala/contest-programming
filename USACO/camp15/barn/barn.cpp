#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXN = 200;
const int MAXK = 60;
const int MAXL = 7;

int N, K, L;
vector<int> adj[MAXN];
int dp[MAXN][MAXK][MAXL];

int W[MAXN];

void dfs(int cur, int prv = -1) {
	for(int k = 0; k <= K; k++) {
		for(int l = 0; l <= L; l++) {
			dp[cur][k][l] = -INF;
		}
	}
	dp[cur][0][L] = 0;
	dp[cur][1][0] = W[cur];

	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;

		dfs(nxt, cur);

		for(int k1 = K; k1 >= 0; k1--) {
			for(int l1 = 0; l1 <= L; l1++) {
				if(dp[cur][k1][l1] == -INF) {
					continue;
				}
				assert(dp[cur][k1][l1] >= 0);
				for(int l2 = 0; l2 < L; l2++) {
					assert(dp[nxt][0][l2] == -INF);
				}
				assert(dp[nxt][0][L] == 0);
				for(int k2 = 1; k1 + k2 <= K; k2++) {
					for(int l2 = 0; l2 <= L; l2++) {
						if(dp[nxt][k2][l2] == -INF) continue;
						assert(dp[nxt][k2][l2] >= 0);
						if(l1 + l2 + 1 <= L) continue;
						int nk = k1 + k2;
						int nl = min(l1, l2 + 1);
						assert(nk <= K);
						assert(nl <= L);
						dp[cur][nk][nl] = max(dp[cur][nk][nl], dp[cur][k1][l1] + dp[nxt][k2][l2]);
					}
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K >> L;
	for(int i = 0; i < N; i++) {
		cin >> W[i];
	}
	for(int i = 0; i < N - 1; i++) {
		int u, v; cin >> u >> v;
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	for(int i = 0; i < N; i++) {
		for(int k = 0; k <= K; k++) {
			for(int l = 0; l <= L; l++) {
				dp[i][k][l] = -INF;
			}
		}
		dp[i][0][L] = 1;
	}
	dfs(0);

	int res = -INF;
	for(int i = 0; i <= L; i++) {
		assert(dp[0][K][i] == -INF || dp[0][K][i] >= 0);
		res = max(res, dp[0][K][i]);
	}

	if(res == -INF) cout << -1 << '\n';
	else cout << res << '\n';

}

