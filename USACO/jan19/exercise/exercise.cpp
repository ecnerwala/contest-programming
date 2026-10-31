#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;
int N;
vector<int> adj[MAXN];
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

const int MAXM = 2.1e5;
int M;

ll ans = 0;

ll edgeSum[MAXN];
ll topCnt[MAXN];
void dfs2(int cur, int prv = 0) {
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs2(nxt, cur);
	}
	ans += edgeSum[cur] * topCnt[cur];
	ans += topCnt[cur] * (topCnt[cur] - 1) / 2;
	edgeSum[prv] += edgeSum[cur];
}

int getAnc(int a, int l) {
	for (int i = 0; (1 << i) <= l; i++) {
		if (l & (1 << i)) {
			a = par[a][i];
		}
	}
	return a;
}

int lca(int a, int b) {
	if (depth[a] > depth[b]) {
		a = getAnc(a, depth[a] - depth[b]);
	} else {
		b = getAnc(b, depth[b] - depth[a]);
	}
	assert(depth[a] == depth[b]);
	if (a == b) return a;

	int i = 0;
	while (par[a][i] != par[b][i]) i++;
	assert(par[a][i] == par[b][i]);
	while (i > 0) {
		i--;
		if (par[a][i] != par[b][i]) {
			a = par[a][i];
			b = par[b][i];
		}
	}
	assert(par[a][i] == par[b][i]);
	assert(i == 0);
	return par[a][0];
}

// number of pairs of paths that share an edge
int main() {
	if(fopen("exercise.in", "r")) {
		auto res = freopen("exercise.in", "r", stdin);
		assert(res);
		res = freopen("exercise.out", "w", stdout);
		assert(res);
	}
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	assert(N >= 1);
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs(1);

	M -= N-1;
	vector<pair<int, int>> tips;
	for (int p = 0; p < M; p++) {
		int a, b; cin >> a >> b;
		int c = lca(a, b);
		int pa = 0, pb = 0;
		if (c != a) {
			pa = getAnc(a, depth[a] - depth[c] - 1);
			edgeSum[a] ++;
			edgeSum[pa] --;
			topCnt[pa] ++;
		}
		if (c != b) {
			pb = getAnc(b, depth[b] - depth[c] - 1);
			edgeSum[b] ++;
			edgeSum[pb] --;
			topCnt[pb] ++;
		}
		//cerr << a << ' ' << b << ' ' << c << ' ' << pa << ' ' << pb << '\n';
		if (c != a && c != b) {
			assert(pa && pb);
			tips.emplace_back(min(pa, pb), max(pa, pb));
		}
	}
	dfs2(1);
	sort(tips.begin(), tips.end());
	for (auto it = tips.begin(); it != tips.end(); ) {
		auto jt = it;
		while (jt != tips.end() && *it == *jt) jt++;
		ll cnt = jt - it;
		ans -= cnt * (cnt-1) / 2;
		it = jt;
	}
	cout << ans << '\n';

	return 0;
}
