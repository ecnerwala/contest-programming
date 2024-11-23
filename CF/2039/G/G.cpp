#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	std::vector<int> max_path(N);
	[&](this auto&& self, int cur, int prv) -> int {
		int r = 0;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			r = std::max(r, self(nxt, cur)+1);
		}
		return max_path[cur] = r;
	}(0, -1);
	[&](this auto&& self, int cur, int prv, int par_v) -> void {
		int best_1 = par_v;
		int best_2 = 0;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			int cnd = max_path[nxt] + 1;
			if (cnd > best_2) {
				std::swap(best_2, cnd);
				if (best_2 > best_1) {
					std::swap(best_1, best_2);
				}
			}
		}

		max_path[cur] = best_1 + best_2 + 1;

		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			int cnd = max_path[nxt] + 1;
			self(nxt, cur, (cnd == best_1 ? best_2 : best_1) + 1);
		}
	}(0, -1, 0);

	for (int i = 0; i < N; i++) {
		cerr << i << ' ' << max_path[i] << '\n';
	}

	return 0;
}
