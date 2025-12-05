#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;

		std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; int64_t c; cin >> u >> v >> c; u--, v--;
			adj[u].push_back({v, c});
			adj[v].push_back({u, c});
		}
		std::vector<int> depth(N, -1);
		std::vector<int> lowval(N, -1);
		std::vector<int64_t> tot(N, -1);
		bool good = true;
		[&](this auto&& self, int cur, int prv) -> void {
			depth[cur] = prv == -1 ? 0 : (depth[prv] + 1);
			lowval[cur] = depth[cur];
			tot[cur] = A[cur];
			for (auto [nxt, c] : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					self(nxt, cur);
					if (lowval[nxt] == depth[nxt] && c < tot[nxt]) {
						good = false;
					}
					tot[cur] += tot[nxt];
					lowval[cur] = std::min(lowval[cur], lowval[nxt]);
				} else if (depth[nxt] < depth[cur]) {
					lowval[cur] = std::min(lowval[cur], depth[nxt]);
				}
			}
		}(0, -1);
		cout << (good ? "TAK" : "NIE") << '\n';
	}

	return 0;
}
