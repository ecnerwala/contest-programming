#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
vector<int> adj[MAXN];
int par[MAXN][20];
int depth[MAXN];

int ans[MAXN];

void dfs(int cur, int prv) {
	depth[cur] = depth[prv]+1;
	par[cur][0] = prv;
	for (int i = 1; i < 20; i++) {
		par[cur][i] = par[par[cur][i-1]][i-1];
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);
	}
}

int lca(int a, int b) {
	if (depth[a] < depth[b]) swap(a,b);
	for (int i = 0; (1 << i) <= (depth[a] - depth[b]); i++) {
		if ((depth[a] - depth[b]) & (1 << i)) {
			a = par[a][i];
		}
	}
	if (a == b) return a;
	int i = 0;
	while (par[a][i] != par[b][i]) i++;
	while (i > 0) {
		--i;
		if (par[a][i] != par[b][i]) {
			a = par[a][i];
			b = par[b][i];
		}
	}
	return par[a][0];
}

void dfs2(int cur, int prv) {
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs2(nxt, cur);
		ans[cur] += ans[nxt];
	}
}

void go() {
	dfs(1,0);
	for (int i = 1; i <= N; i++) {
		ans[i] = 1;
	}
	for (int i = 1; i+1 <= N; i++) {
		int l = lca(i, i+1);
		//cerr << i << ' ' << i+1 << ' ' << l << '\n';
		ans[l] --;
	}
	dfs2(1, 0);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num++) {
		//cerr << "Case #" << case_num << ":" << '\n';
		cin >> N;

		for (int i = 0; i < N-1; i++) {
			int u, v; cin >> u >> v;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		go();

		cout << "Case #" << case_num << ":";
		for (int i = 1; i <= N; i++) {
			cout << ' ' << ans[i];
		}
		cout << '\n';

		for (int i = 1; i <= N; i++) {
			adj[i] = {};
		}
	}

	return 0;
}
