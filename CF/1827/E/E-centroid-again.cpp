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
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<std::vector<int>> routes(N);
		for (int m = 0; m < M; m++) {
			int x, y; cin >> x >> y; x--, y--;
			if (adj[x].size() == 1) routes[x].push_back(y);
			if (adj[y].size() == 1) routes[y].push_back(x);
		}

		auto get_unreachable = [&]() -> std::optional<std::array<int, 2>> {
			std::vector<int> alive(N, 1);
			std::vector<int> killer(N, -1);
			std::vector<int> sz(N);
			std::vector<int> cur_cc(N);
			int centroid = 0;
			while (true) {
				assert(alive[centroid]);
				std::y_combinator([&](auto self, int cur, int prv) -> int {
					if (!alive[cur]) return 0;
					int r = 1;
					for (int nxt : adj[cur]) {
						if (nxt == prv) continue;
						r += self(nxt, cur);
					}
					sz[cur] = r;
					return r;
				})(centroid, -1);
				int tot_sz = sz[centroid];
				while (true) {
					bool found = false;
					for (int nxt : adj[centroid]) {
						if (!alive[nxt]) continue;
						if (sz[nxt] > sz[centroid]) continue;
						if (2 * sz[nxt] > tot_sz) {
							centroid = nxt;
							found = true;
							break;
						}
					}
					if (!found) break;
				}

				// now, we have the centroid
				cur_cc[centroid] = centroid;
				for (int rt : adj[centroid]) {
					std::y_combinator([&](auto self, int cur, int prv) -> void {
						cur_cc[cur] = rt;
						for (int nxt : adj[cur]) {
							if (nxt == prv) continue;
							self(nxt, cur);
						}
					})(rt, centroid);
				}

				int bad_rt = -1;
				int bad_ch = -1;
				for (int rt : adj[centroid]) {
					int cur_bad_ch = -1;
					std::y_combinator([&](auto self, int cur, int prv) -> bool {
						if (adj[cur].size() == 1) {
							bool found = false;
							for (int y : routes[cur]) {
								if (cur_cc[y] != rt) {
									found = true;
									break;
								}
							}
							if (!found) {
								cur_bad_ch = cur;
								return true;
							}
						}
						for (int nxt : adj[cur]) {
							if (nxt == prv) continue;
							if (self(nxt, cur)) return true;
						}
						return false;
					})(rt, centroid);

					if (cur_bad_ch != -1) {
						if (bad_rt != -1) {
							return std::array<int, 2>{bad_ch, cur_bad_ch};
						}
						bad_rt = rt;
						bad_ch = cur_bad_ch;
					}
				}

				if (bad_rt == -1) return std::nullopt;
				alive[centroid] = false;
				killer[centroid] = bad_ch;

				if (!alive[bad_rt]) {
					return std::array<int, 2>{bad_ch, killer[bad_rt]};
				}
				centroid = bad_rt;
			}
		};

		auto ans = get_unreachable();
		if (ans) {
			cout << "NO" << '\n';
			cout << (*ans)[0]+1 << ' ' << (*ans)[1]+1 << '\n';
		} else {
			cout << "YES" << '\n';
		}
	}

	return 0;
}
