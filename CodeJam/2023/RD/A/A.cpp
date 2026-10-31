#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		std::array<int, 2> N; int Q; cin >> N[0] >> N[1] >> Q;
		std::array<int64_t, 2> tot_dist;
		std::array<std::vector<int64_t>, 2> loc_dist;

		for (int z = 0; z < 2; z++) {
			loc_dist[z].resize(N[z]);
			std::vector<std::vector<int>> adj(N[z]);
			for (int v = 0; v+1 < N[z]; v++) {
				int u; cin >> u; u--;
				adj[u].push_back(v);
				adj[v].push_back(u);
			}
			std::vector<int> sz(N[z]);

			std::y_combinator([&](auto self, int cur, int prv) -> void {
				sz[cur] = 1;
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur);
					loc_dist[z][0] += sz[nxt];
					sz[cur] += sz[nxt];
				}
			})(0, -1);

			std::y_combinator([&](auto self, int cur, int prv) -> void {
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					loc_dist[z][nxt] = loc_dist[z][cur] - sz[nxt] + (N[z] - sz[nxt]);
					self(nxt, cur);
				}
			})(0, -1);

			tot_dist[z] = std::accumulate(loc_dist[z].begin(), loc_dist[z].end(), int64_t(0)) / 2;
		}

		int64_t denom = int64_t(N[0] + N[1]) * int64_t(N[0] + N[1] - 1) / 2;
		cout << "Case #" << case_num << ": ";
		for (int q = 0; q < Q; q++) {
			int a, b; cin >> a >> b; a--, b--;
			int64_t r = int64_t(loc_dist[0][a]) * N[1] + int64_t(N[0]) * int64_t(N[1]) + int64_t(loc_dist[1][b]) * N[0];
			r += tot_dist[0] + tot_dist[1];
			double res = double(r) / double(denom);
			cout << fixed << setprecision(12) << res << " \n"[q+1==Q];
		}
	}

	return 0;
}
