#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4e4;
int N;
vector<int> adj[MAXN];
int par[MAXN][20];
int depth[MAXN];

void dfs(int cur, int prv = 0) {
	par[cur][0] = prv;
	for(int i = 0; par[cur][i]; i++) {
		assert(i + 1 < 20);
		par[cur][i + 1] = par[par[cur][i]][i];
	}

	for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if(nxt == prv) continue;
		depth[nxt] = depth[cur] + 1;
		dfs(nxt, cur);
	}
}

int lca(int a, int b) {
	if(depth[a] > depth[b]) swap(a, b);
	assert(depth[b] >= depth[a]);
	for(int i = 0; depth[b] > depth[a]; i++) {
		if((depth[b] - depth[a]) & (1 << i)) {
			assert(depth[par[b][i]] == depth[b] - (1 << i));
			b = par[b][i];
		}
	}

	assert(depth[b] == depth[a]);

	if(a == b) return a;

	for(int i = 19; i >= 0; i--) {
		if(par[a][i] != par[b][i]) {
			a = par[a][i];
			b = par[b][i];
		}
	}
	assert(a != b);
	assert(par[a][0] == par[b][0]);
	return par[a][0];
}

int dist(int a, int b) {
	return depth[a] + depth[b] - 2 * depth[lca(a, b)];
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	assert(N < MAXN);
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	dfs(0);

	int M; cin >> M;
	long long res = 0;
	int cur = 0;
	for(int i = 0; i < M; i++) {
		int nxt; cin >> nxt; nxt--;
		res += dist(cur, nxt);
		cur = nxt;
	}
	cout << res << '\n';
	return 0;
}
