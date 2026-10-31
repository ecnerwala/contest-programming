#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
vector<int> adj[MAXN];

void dfs(int cur, int prv, vector<int>& dist) {
	if (prv == -1) dist[cur] = 0;
	else dist[cur] = dist[prv]+1;

	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur, dist);
	}
}

vector<int> getDists(int st) {
	vector<int> res(N);
	dfs(st, -1, res);
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int M;
	cin >> N >> M;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	vector<int> C(M);
	for (int j = 0; j < M; j++) {
		cin >> C[j]; C[j] --;
	}

	vector<int> d0 = getDists(C[0]);
	int x = *max_element(C.begin(), C.end(), [&](int i, int j) { return d0[i] < d0[j]; });
	vector<int> dx = getDists(x);
	int y = *max_element(C.begin(), C.end(), [&](int i, int j) { return dx[i] < dx[j]; });
	vector<int> dy = getDists(y);

	int diam = dx[y];
	if (diam & 1) {
		cout << "NO" << '\n';
		exit(0);
	}
	int z = 0;
	while (!(dx[z] == diam/2 && dy[z] == diam/2)) {
		z++;
	}

	// the center is z
	vector<int> dz = getDists(z);

	if (
			dz[*max_element(C.begin(), C.end(), [&](int i, int j) { return dz[i] < dz[j]; })] == 
			dz[*min_element(C.begin(), C.end(), [&](int i, int j) { return dz[i] < dz[j]; })]
	   ) {
		cout << "YES" << '\n';
		cout << z+1 << '\n';
		exit(0);
	} else {
		cout << "NO" << '\n';
		exit(0);
	}

	return 0;
}
