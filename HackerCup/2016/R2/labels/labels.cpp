#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

const int MAXN = 2e3;
const int MAXK = 32;
int N, K;
vector<int> adj[MAXN];

ll P;
ll C[MAXN][MAXK];

ll dp[MAXN][MAXK][MAXK]; // cur, cur_c, prv_c

unordered_map<int, ll> mp[2];

void dfs(int cur, int prv) {
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		dfs(nxt, cur);
	}

	// my own color
	for(int k = 0; k < K; k++) {
		ll Pval = P + C[cur][k];
		for(int nxt : adj[cur]) {
			if(nxt == prv) continue;
			ll best = dp[nxt][0][k];
			for(int nk = 0; nk < K; nk++) {
				best = min(best, dp[nxt][nk][k]);
			}
			Pval += best;
		}
		for(int pk = 0; pk < K; pk++) {
			dp[cur][k][pk] = Pval;
		}

		if(int(adj[cur].size()) > K) continue;

		mp[0].clear();
		mp[1].clear();
		int d = 0;
		mp[d][0] = C[cur][k];
		for(int nxt : adj[cur]) {
			if(nxt == prv) continue;
			mp[!d].clear();
			for(auto it : mp[d]) {
				int m = it.first;
				ll v = it.second;
				for(int nk = 0; nk < K; nk++) {
					if(m & (1 << nk)) continue;
					int nm = m | (1 << nk);
					ll nv = v + dp[nxt][nk][k];
					if(mp[!d].count(nm)) mp[!d][nm] = min(mp[!d][nm], nv);
					else mp[!d][nm] = nv;
				}
			}
			d = !d;
		}

		for(auto it : mp[d]) {
			int m = it.first;
			ll v = it.second;
			for(int pk = 0; pk < K; pk++) {
				if(m & (1 << pk)) continue;
				dp[cur][k][pk] = min(dp[cur][k][pk], v);
			}
		}
	}
}

ll go() {
	cin >> N >> K >> P;
	for(int i = 0; i < N; i++) {
		for(int k = 0; k < K; k++) {
			cin >> C[i][k];
		}
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
		ll res = C[0][0];
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
	ll res = INF;
	for(int k1 = 0; k1 < K; k1++) {
		for(int k2 = 0; k2 < K; k2++) {
			res = min(res, dp[ch][k1][k2] + C[root][k2]);
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);

	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		ll res = go();
		cout << "Case #" << T << ": " << res << '\n';
	}
	return 0;
}
