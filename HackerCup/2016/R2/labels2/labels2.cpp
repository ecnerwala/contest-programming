#include<bits/stdc++.h>
using namespace std;

const int INF = 2e9 + 200;

const int MAXN = 2e3;
const int MAXK = 32;
int N, K;
vector<int> adj[MAXN];

int P;
int C[MAXN][MAXK];

int dp[MAXN][MAXK][MAXK]; // cur, cur_c, prv_c

vector<int> vals[31];
int *mp;

void dfs(int cur, int prv) {
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		dfs(nxt, cur);
	}

	// my own color
	for(int k = 0; k < K; k++) {
		int Pval = P + C[cur][k];
		for(int nxt : adj[cur]) {
			if(nxt == prv) continue;
			int best = dp[nxt][0][k];
			for(int nk = 0; nk < K; nk++) {
				best = min(best, dp[nxt][nk][k]);
			}
			Pval += best;
		}
		for(int pk = 0; pk < K; pk++) {
			dp[cur][k][pk] = Pval;
		}

		if(int(adj[cur].size()) > K) continue;

		mp[0] = C[cur][k];

		int d = 0;
		for(int nxt : adj[cur]) {
			if(nxt == prv) continue;
			d++;
			for(int m : vals[d]) {
				int res = INF;
				for(int nk = 0; nk < K; nk++) {
					if(!(m & (1 << nk))) continue;
					res = min(res, mp[m ^ (1 << nk)] + dp[nxt][nk][k]);
				}
				mp[m] = res;
			}
		}

		for(int m : vals[d]) {
			int v = mp[m];
			for(int pk = 0; pk < K; pk++) {
				if(m & (1 << pk)) continue;
				dp[cur][k][pk] = min(dp[cur][k][pk], v);
			}
		}
	}
}

int go() {
	cin >> N >> K >> P;
	for(int i = 0; i < N; i++) {
		for(int k = 0; k < K; k++) {
			cin >> C[i][k];
		}
	}
	for(int i = 0; i <= K; i++) {
		vals[i].clear();
	}
	for(int m = 0; m < (1 << K); m++) {
		int d = 0;
		for(int v = m; v; v &= v - 1) d++;
		vals[d].push_back(m);
	}
	for(int i = 0; i < N; i++) {
		adj[i].clear();
	}
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	if(N == 1) {
		int res = C[0][0];
		for(int i = 0; i < K; i++) {
			res = min(res, C[0][i]);
		}
		return res;
	}

	int root = -1;
	for(int i = 0; i < N; i++) {
		if(int(adj[i].size()) == 1) {
			root = i;
			break;
		}
	}
	// root ourselves at a leaf
	assert(root != -1);
	int ch = adj[root][0];

	dfs(ch, root);
	int res = INF;
	for(int k1 = 0; k1 < K; k1++) {
		for(int k2 = 0; k2 < K; k2++) {
			res = min(res, dp[ch][k1][k2] + C[root][k2]);
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);

	mp = new int[1 << 30];

	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		int res = go();
		cout << "Case #" << T << ": " << res << '\n';
	}
	return 0;
}
