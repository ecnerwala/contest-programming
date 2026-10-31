#include<bits/stdc++.h>
using namespace std;

template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }

using ll = long long;

const int MAXN = 3.1e5;
const int MAXK = 3.1e5;

const int MAXNK = 9.1e5;

int N, K;
vector<int> adj[MAXN];

int depth[MAXN];
int par[MAXN][20];
void dfs(int cur, int prv) {
	depth[cur] = depth[prv] + 1;
	par[cur][0] = prv;
	for (int i = 0; par[cur][i]; i++) {
		par[cur][i+1] = par[par[cur][i]][i];
	}

	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);
	}
}

int lca(int a, int b) {
	assert(a && b);
	if (depth[a] > depth[b]) swap(a, b);
	assert(depth[b] >= depth[a]);
	for (int i = 0; (depth[b]-depth[a]); i++) {
		if ((1<<i) & (depth[b] - depth[a])) {
			b = par[b][i];
		}
	}
	if (a == b) return a;
	int i = 0;
	while (par[a][i] != par[b][i]) i++;
	while (i > 0) {
		--i;
		if (par[a][i] != par[b][i]) {
			a = par[a][i], b = par[b][i];
		}
	}
	assert(par[a][0] == par[b][0]);
	assert(par[a][0] != 0);
	return par[a][0];
}

int P[MAXN];
int vDepth[MAXN];
int lcaDepth[MAXN];

ll dp[MAXNK];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> P[i];
	}
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	dfs(1, 0);

	for (int i = 0; i < N; i++) {
		vDepth[i] = depth[P[i]];
	}
	for (int i = 0; i+1 < N; i++) {
		lcaDepth[i] = depth[lca(P[i], P[i+1])];
		assert(lcaDepth[i] <= vDepth[i]);
		assert(lcaDepth[i] <= vDepth[i+1]);
	}

	const ll INF = 1e18;
	for (int i = 0; i <= N; i++) {
		for (int k = 0; k <= K; k++) {
			dp[i*(K+1)+k] = INF;
		}
	}

	dp[0*(K+1)+0] = 0;

	for (int i = 0; i < N; i++) {
		for (int k = 0; k <= K; k++) {
			setmin(dp[(i+1)*(K+1)+k], dp[i*(K+1)+k]);
			if (k+1 <= K) {
				setmin(dp[(i+1)*(K+1)+k+1], dp[i*(K+1)+k] + vDepth[i]);
				if (i+1 < N) {
					setmin(dp[(i+2)*(K+1)+k+1], dp[i*(K+1)+k] + lcaDepth[i]);
				}
			}
		}
	}

	ll ans = dp[N*(K+1)+K];
	assert(ans < INF);
	cout << ans << '\n';

	return 0;
}
