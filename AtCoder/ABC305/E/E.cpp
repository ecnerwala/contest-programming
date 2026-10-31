#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	std::vector<int> dist(N, -1);
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<std::vector<int>> guards(N+1);
	for (int k = 0; k < K; k++) {
		int v, d; cin >> v >> d; v--;
		guards[d].push_back(v);
	}
	
	std::vector<int> q; q.reserve(N);
	int idx = 0;
	for (int d = N; d >= 0; d--) {
		for (int g : guards[d]) {
			if (dist[g] == -1) {
				dist[g] = d;
				q.push_back(g);
			}
		}
		if (d == 0) break;
		int en = int(q.size());
		for (; idx < en; idx++) {
			int cur = q[idx];
			for (int nxt : adj[cur]) {
				if (dist[nxt] == -1) {
					dist[nxt] = d-1;
					q.push_back(nxt);
				}
			}
		}
	}

	std::vector<int> res; res.reserve(N);
	for (int i = 0; i < N; i++) {
		if (dist[i] != -1) {
			res.push_back(i);
		}
	}
	cout << res.size() << '\n';
	for (int z = 0; z < int(res.size()); z++) {
		cout << res[z]+1 << " \n"[z+1==int(res.size())];
	}

	return 0;
}
