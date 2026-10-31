#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
const int MAXM = 1.1e5;
int N, M;
pair<int, int> E[MAXM];

vector<pair<int, int>> adj[MAXN];
int outDeg[MAXN]; // just parity

bool vis[MAXN];
void dfs(int cur) {
	assert(!vis[cur]);
	vis[cur] = true;
	for (auto it : adj[cur]) {
		int nxt = it.first;
		int e = it.second;
		if (vis[nxt]) continue;
		dfs(nxt);
		if (outDeg[nxt]) {
			swap(E[e].first, E[e].second);
			outDeg[nxt] ^= 1;
			outDeg[cur] ^= 1;
		}
		assert(outDeg[nxt] == 0);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int e = 0; e < M; e++) {
		cin >> E[e].first >> E[e].second;
		E[e].first --, E[e].second --;
	}

	if (M % 2 == 1) {
		cout << -1 << '\n';
		exit(0);
	}

	for (int e = 0; e < M; e++) {
		int u, v; tie(u, v) = E[e];
		adj[u].emplace_back(v, e);
		adj[v].emplace_back(u, e);
		outDeg[u] ^= 1;
	}

	dfs(0);
	for (int e = 0; e < M; e++) {
		cout << E[e].first+1 << ' ' << E[e].second+1 << '\n';
	}

	return 0;
}
