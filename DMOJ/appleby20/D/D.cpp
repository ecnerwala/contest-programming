#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<pair<int, int>> edges(M);
	for (auto& it : edges) {
		cin >> it.first >> it.second;
		it.first--;
		it.second--;
	}
	vector<vector<bool>> adj(N, vector<bool>(N));
	for (auto it : edges) {
		adj[it.first][it.second] = true;
		adj[it.second][it.first] = true;
	}

	array<int, 3> ans_3{N, N, N};
	for (auto it : edges) {
		int u = it.first;
		int v = it.second;
		for (int i = 0; i < N; i++) {
			if (i == u || i == v) continue;
			if (adj[u][i] && adj[v][i]) {
				array<int, 3> cnd{i, u, v};
				sort(cnd.begin(), cnd.end());
				ans_3 = min(ans_3, cnd);
			}
		}
	}
	if (ans_3[0] != N) {
		cout << 3 << '\n';
		cout << ans_3[0]+1 << ' ' << ans_3[1]+1 << ' ' << ans_3[2]+1 << '\n';
		exit(0);
	}

	array<int, 4> ans_4{N,N,N,N};
	for (auto it : edges) {
		for (auto jt : edges) {
			if (it.first != jt.first && it.first != jt.second && it.second != jt.first && it.second != jt.second) {
				array<int, 4> cnd{it.first, it.second, jt.first, jt.second};
				sort(cnd.begin(), cnd.end());
				ans_4 = min(ans_4, cnd);
			}
		}
	}

	if (ans_4[0] != N) {
		cout << 4 << '\n';
		cout << ans_4[0]+1 << ' ' << ans_4[1]+1 << ' ' << ans_4[2]+1 << ' ' << ans_4[3]+1 << '\n';
		exit(0);
	}
	cout << -1 << '\n';

	return 0;
}
