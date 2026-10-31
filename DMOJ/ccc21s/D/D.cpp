#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;
	vector<vector<int>> adj(N);
	for (int i = 0; i < M; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[v].push_back(u);
	}

	vector<int> dist(N, 2*N+1);
	vector<int> q; q.reserve(N);
	dist[N-1] = 0;
	q.push_back(N-1);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		for (int nxt : adj[cur]) {
			if (dist[nxt] <= dist[cur]+1) continue;
			dist[nxt] = dist[cur]+1;
			q.push_back(nxt);
		}
	}

	set<pair<int, int>> mp;
	vector<int> S(N);
	for (int i = 0; i < N; i++) {
		cin >> S[i]; S[i]--;
		S[i] = dist[S[i]];
		mp.insert({S[i]+i, i});
	}
	while (Q--) {
		int x, y; cin >> x >> y; x--, y--;
		mp.erase({S[x]+x, x});
		mp.erase({S[y]+y, y});
		swap(S[x], S[y]);
		mp.insert({S[x]+x, x});
		mp.insert({S[y]+y, y});
		cout << mp.begin()->first << '\n';
	}

	return 0;
}
