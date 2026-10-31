#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> C(N);
	for (auto& c : C) { cin >> c; c--; }

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	auto bfs = [&](int st) -> std::vector<int> {
		std::vector<int> dist(N, -1);
		std::vector<int> q; q.reserve(N);
		dist[st] = 0;
		q.push_back(st);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt : adj[cur]) {
				if (dist[nxt] == -1) {
					dist[nxt] = dist[cur] + 1;
					q.push_back(nxt);
				}
			}
		}
		return dist;
	};
	auto d0 = bfs(0);
	auto d1 = bfs(N-1);

	int ans = -1;
	for (int z = 0; z < 2; z++, std::swap(d0, d1)) {
		std::vector<int> best(N, -1);
		for (int i = 0; i < N; i++) {
			if (best[C[i]] != -1 && d1[i] != -1) {
				int cnd = best[C[i]] + d1[i];
				if (ans == -1 || cnd < ans) ans = cnd;
			}
			if (d0[i] != -1) {
				if (best[C[i]] == -1 || d0[i] < best[C[i]]) best[C[i]] = d0[i];
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
