#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> bounds(N);
		for (auto& [l, r] : bounds) cin >> l >> r;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) { int u, v; cin >> u >> v; u--, v--; }

		std::vector<int> sz(N);
		[&](this auto&& self, int cur, int prv) -> int {
			int s = 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				s += self(nxt, cur);
			}
			return sz[cur] = s;
		}(0, -1);
		int R = 0;
		while (true) {
			int nxt = -1;
			for (int a : adj[R]) {
				if (sz[a] > sz[R]) continue;
				if (sz[a] * 2 > N) {
					nxt = a;
					break;
				}
			}
			if (nxt == -1) break;
			R = nxt;
		}

		std::vector<int> val(N);
		int64_t ans = 0;
		[&](this auto&& self, int cur, int prv) -> void {
			int v = bounds[cur][0];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				v = std::max(v, val[nxt]);
			}
			v = std::min(v, bounds[cur][1]);
			val[cur] = v;
			cerr << cur << ' ' << v << '\n';
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (val[nxt] > val[cur]) ans += val[nxt] - val[cur];
			}
		}(R, -1);
		ans += val[R];
		cout << ans << '\n';
	}

	return 0;
}
