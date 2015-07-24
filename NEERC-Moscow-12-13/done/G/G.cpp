#include<bits/stdc++.h>
using namespace std;
const int MAXN = 2e5;

vector<int> in[MAXN];
vector<int> out[MAXN];

int par[MAXN][20];
int dist[MAXN][2]; // 1 is down, 0 is up
int depth[MAXN];

void dfs(int cur, int prv = 0) {
	for(int i = 0; i + 1 < 20; i++) {
		par[cur][i + 1] = par[par[cur][i]][i];
	}
	for(int nxt : out[cur]) {
		if(nxt == prv) continue;

		par[nxt][0] = cur;
		depth[nxt] = depth[cur] + 1;

		dist[nxt][1] = dist[cur][1] + 1;
		dist[nxt][0] = 0;
		dfs(nxt, cur);
	}

	for(int nxt : in[cur]) {
		if(nxt == prv) continue;

		par[nxt][0] = cur;
		depth[nxt] = depth[cur] + 1;

		dist[nxt][0] = dist[cur][0] + 1;
		dist[nxt][1] = 0;
		dfs(nxt, cur);
	}
}

int lca(int a, int b) {
	if(depth[a] < depth[b]) swap(a, b);
	for(int i = 19; i >= 0; i--) {
		if(depth[a] >= depth[b] + (1 << i)) a = par[a][i];
	}
	assert(depth[a] == depth[b]);
	if(a == b) return a;
	for(int i = 19; i >= 0; i--) {
		if(par[a][i] != par[b][i]) a = par[a][i], b = par[b][i];
	}
	return par[a][0];
}

bool query(int x, int y) {
	int z = lca(x, y);
	return (dist[x][0] >= depth[x] - depth[z]) && (dist[y][1] >= depth[y] - depth[z]);
}

int main() {
	ios_base::sync_with_stdio(0);
	int N; cin >> N;
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b;
		out[a].push_back(b);
		in[b].push_back(a);
	}
	dfs(1);
	int M; cin >> M;
	for(int i = 0; i < M; i++) {
		int x, y; cin >> x >> y;
		cout << (query(x, y) ? "Yes" : "No") << '\n';
	}
	return 0;
}
