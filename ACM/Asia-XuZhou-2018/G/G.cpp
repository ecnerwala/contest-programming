#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll MOD = ll(1e9) + 7;

const int MAXN = 4e5;
int N;
vector<int> adj[MAXN];
int vertCnt[MAXN];
int edgeCnt[MAXN];
int M;

int K;


int depth[MAXN];
int par[MAXN][20];
void dfs(int cur, int prv = 0) {
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
	if (depth[a] > depth[b]) swap(a,b);
	assert(depth[b] >= depth[a]);
	for (int i = 0; (1 << i) <= (depth[b] - depth[a]); i++) {
		if ((depth[b] - depth[a]) & (1 << i)) {
			b = par[b][i];
		}
	}
	assert(depth[a] == depth[b]);
	if (a == b) return a;

	int i = 0;
	while (par[a][i] != par[b][i]) i++;

	assert(a != b);
	assert(par[a][i] == par[b][i]);
	while (i > 0) {
		assert(a != b);
		assert(par[a][i] == par[b][i]);
		i--;
		if (par[a][i] != par[b][i]) {
			a = par[a][i];
			b = par[b][i];
		}
		assert(a != b);
		assert(par[a][i] == par[b][i]);
	}
	assert(a != b);
	assert(par[a][i] == par[b][i]);
	assert(i == 0);
	return par[a][i];
}

const int MAXM = 4e5;
ll chooseK[MAXM];

ll res = 0;

void dfs2(int cur, int prv = 0) {
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs2(nxt, cur);
		vertCnt[cur] += vertCnt[nxt];
		edgeCnt[cur] += edgeCnt[nxt];
	}
	assert(vertCnt[cur] >= edgeCnt[cur]);
	res += chooseK[vertCnt[cur]] - chooseK[edgeCnt[cur]];
	res %= MOD;
	if (res < 0) res += MOD;
}

ll invmod(ll a, ll m) {
	a %= m;
	if (a < 0) a += m;
	assert(a);
	if (a == 1) return 1;
	return m - invmod(m, a) * m / a;
}
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N >> M >> K;
		for (int i = 0; i < N-1; i++) {
			int u, v; cin >> u >> v;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		memset(par, 0, sizeof(par));
		memset(depth, 0, sizeof(depth));
		memset(vertCnt, 0, sizeof(vertCnt));
		memset(edgeCnt, 0, sizeof(edgeCnt));
		memset(chooseK, 0, sizeof(chooseK));

		dfs(1);

		for (int m = 0; m < M; m++) {
			int a, b; cin >> a >> b;
			int c = lca(a, b);
			edgeCnt[a] += 1;
			edgeCnt[b] += 1;
			edgeCnt[c] -= 2;

			vertCnt[a] += 1;
			vertCnt[b] += 1;
			vertCnt[c] -= 1;
			vertCnt[par[c][0]] -= 1;
		}


		for (int m = 0; m < K; m++) {
			chooseK[m] = 0;
		}

		chooseK[K] = 1;
		for (int m = K+1; m <= M; m++) {
			chooseK[m] = chooseK[m-1];
			chooseK[m] *= m;
			chooseK[m] %= MOD;
			chooseK[m] *= invmod(m-K, MOD);
			chooseK[m] %= MOD;
		}

		res = 0;
		dfs2(1);

		res %= MOD;
		if (res < 0) res += MOD;
		cout << res << '\n';

		for (int i = 1; i <= N; i++) {
			adj[i] = vector<int>();
		}
	}

	return 0;
}
