#include<bits/stdc++.h>
using namespace std;

const int MAXN = 600;
const int MAXM = 1600;
int N, M;
pair<int, int> edges[MAXM];
int S[MAXM];

vector<pair<int, int>> adj[3][MAXN];
pair<int, int> parE[3][MAXN];
int depth[3][MAXN];
int prv[3][MAXN];
int treeRoot[3][MAXN];

int getPrv(int t, int u) {
	return prv[t][u] == u ? u : (prv[t][u] = getPrv(t, prv[t][u]));
}

void dfs(int t, int cur) {
	for (auto e : adj[t][cur]) {
		if (e == parE[t][cur]) continue;
		int nxt = e.second;
		parE[t][nxt] = {e.first, cur};
		depth[t][nxt] = depth[t][cur]+1;
		treeRoot[t][nxt] = treeRoot[t][cur];
		dfs(t, nxt);
	}
}

void buildTrees() {
	for (int t = 0; t < 3; t++) {
		for (int i = 1; i <= N; i++) {
			adj[t][i] = {};
			depth[t][i] = 0;
		}
	}
	for (int e = 0; e < M; e++) {
		if (S[e] == -1) continue;
		int t = S[e];
		int u = edges[e].first;
		int v = edges[e].second;
		//cerr << "add edge " << t << ' ' << u << ' ' << v << '\n';
		adj[t][u].push_back({e, v});
		adj[t][v].push_back({e, u});
	}

	for (int t = 0; t < 3; t++) {
		for (int i = 1; i <= N; i++) {
			if (depth[t][i] == 0) {
				depth[t][i] = 1;
				parE[t][i] = {-1, 0};
				treeRoot[t][i] = i;
				dfs(t, i);
			}
		}
	}
	for (int t = 0; t < 3; t++) {
		for (int i = 0; i <= N; i++) {
			prv[t][i] = i;
		}
	}
}

bool augment(int t, int u, int v) {
	if (treeRoot[t][u] != treeRoot[t][v]) return true;
	vector<int> cnds;
	while (true) {
		u = getPrv(t, u);
		v = getPrv(t, v);
		if (u == v) break;
		if (depth[t][v] >= depth[t][u]) swap(u,v);
		int e = parE[t][u].first;
		prv[t][u] = parE[t][u].second;
		cnds.push_back(e);
	}
	for (int e : cnds) {
		for (int z = 0; z < 3; z++) {
			if (z == t) continue;
			if (augment(z, edges[e].first, edges[e].second)) {
				//cerr << "set " << z << ' ' << e << ' ' << edges[e].first << ' ' << edges[e].second << '\n';
				S[e] = z;
				return true;
			}
		}
	}
	return false;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int e = 0; e < M; e++) {
		cin >> edges[e].first >> edges[e].second;
		S[e] = -1;
	}
	int cnt = 0;
	buildTrees();
	for (int e = 0; e < M; e++) {
		for (int t = 0; t < 3; t++) {
			if (augment(t, edges[e].first, edges[e].second)) {
				//cerr << "set " << t << ' ' << e << ' ' << edges[e].first << ' ' << edges[e].second << '\n';
				S[e] = t;
				cnt++;
				buildTrees();
				break;
			}
		}
	}

	if (cnt == 3 * (N-1)) {
		for (int e = 0; e < M; e++) {
			cout << S[e]+1;
		}
		cout << '\n';
	} else {
		cout << -1 << '\n';
	}
	return 0;
}
