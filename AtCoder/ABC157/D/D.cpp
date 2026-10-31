#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int par[MAXN];
void reset() {
	memset(par, -1, sizeof(par));
}
int getPar(int a) { return par[a] < 0 ? a : (par[a] = getPar(par[a])); }
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	par[a] += par[b];
	par[b] = a;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	reset();
	int N, M, K; cin >> N >> M >> K;
	vector<vector<int>> adj(N);
	for (int i = 0; i < M; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
		merge(u, v);
	}
	for (int i = 0; i < K; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	for (int i = 0; i < N; i++) {
		int p = getPar(i);
		int ans = -par[p];
		ans--;
		for (int j : adj[i]) {
			if (getPar(j) == p) ans--;
		}
		cout << ans << " \n"[i+1==N];
	}

	return 0;
}
