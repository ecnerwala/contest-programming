#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::array<int, 2>> edges(M);
	std::vector<std::vector<int>> adj(N);
	for (auto& [x, y] : edges) {
		cin >> x >> y; x--, y--;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}

	int A, B, C, D; cin >> A >> B >> C >> D; A--, B--, C--, D--;
	for (auto [x, y] : edges) {
		if ((x == C && y == D) || (x == D && y == C)) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	int best_dist = N+1;
	int best_skip = -1;
	for (int skip : {C, D}) {
		std::vector<int> dist(N, -1);
		dist[A] = 0;
		std::vector<int> q; q.reserve(N);
		q.push_back(A);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt : adj[cur]) {
				if (nxt == skip) continue;
				if (dist[nxt] != -1) continue;
				dist[nxt] = dist[cur] + 1;
				q.push_back(nxt);
			}
		}
		if (dist[B] != -1 && dist[B] < best_dist) {
			best_dist = dist[B];
			best_skip = skip;
		}
	}
	if (best_skip == -1) {
		cout << -1 << '\n';
	} else {
		cout << 2 << '\n';
		for (auto [x, y] : edges) {
			if (x == best_skip || y == best_skip) cout << 2 << '\n';
			else cout << 1 << '\n';
		}
	}

	return 0;
}
