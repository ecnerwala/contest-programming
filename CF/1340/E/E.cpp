#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N, M; cin >> N >> M;
	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<vector<int>> dist(N, vector<int>(N, -1));
	for (int i = 0; i < N; i++) {
		vector<int> q; q.reserve(N);
		dist[i][i] = 0;
		q.push_back(i);
		for (int cur : q) {
			for (int nxt : adj[cur]) {
				if (dist[i][nxt] == -1) {
					dist[i][nxt] = dist[i][cur]+1;
					q.push_back(nxt);
				}
			}
		}
	}

	return 0;
}
