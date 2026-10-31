#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];
int par[MAXN];
int sz[MAXN];
void dfs(int cur, int prv) {
	par[cur] = prv;
	sz[cur] = 1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);
		sz[cur] += sz[nxt];
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs(1, 0);
	vector<int> dq;
	for (int cur = N; cur; cur = par[cur]) {
		dq.push_back(cur);
	}
	assert(dq.back() == 1);
	int s = sz[dq[int(dq.size()) / 2 - 1]];
	if (s >= N - s) {
		cout << "Snuke" << '\n';
	} else {
		cout << "Fennec" << '\n';
	}

	return 0;
}
