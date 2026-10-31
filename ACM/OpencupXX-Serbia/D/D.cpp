#include<bits/stdc++.h>

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
		int N; cin >> N;
		vector<vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		auto go = [&]() -> int {
			auto get_farthest = std::y_combinator([&](auto self, int cur, int prv = -1) -> pair<int, int> {
				pair<int, int> res(0, cur);
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					auto cnd = self(nxt, cur);
					cnd.first++;
					res = max(res, cnd);
				}
				return res;
			});

			int diam[2], D;
			diam[0] = get_farthest(0).second;
			tie(D, diam[1]) = get_farthest(diam[0]);

			vector<int> dist[2];
			for (int z = 0; z < 2; z++) {
				dist[z] = vector<int>(N, -1);
				vector<int> q; q.reserve(N);
				dist[z][diam[z]] = 0;
				q.push_back(diam[z]);
				for (int ind = 0; ind < N; ind++) {
					int cur = q[ind];
					for (int nxt : adj[cur]) {
						if (dist[z][nxt] == -1) {
							dist[z][nxt] = dist[z][cur]+1;
							q.push_back(nxt);
						}
					}
				}
			}
			if (N % 2 == 1) {
				// check if there's any other D
				for (int i = 0; i < N; i++) {
					if (i == diam[0] || i == diam[1]) continue;
					if (dist[0][i] == D || dist[1][i] == D) {
						return D;
					}
				}
				return D-1;
			} else {
				assert(N % 2 == 0);
				{
					int cnt0 = 0, cnt1 = 0;
					int cntAll = 0;
					for (int i = 0; i < N; i++) {
						if (i == diam[0] || i == diam[1]) continue;
						bool g0 = dist[0][i] == D;
						bool g1 = dist[1][i] == D;
						cnt0 += g0;
						cnt1 += g1;
						cntAll += g0 || g1;
					}
					if (cnt0 >= 1 && cnt1 >= 1 && cntAll >= 2) {
						return D;
					}
					if (cnt0 >= 1 || cnt1 >= 1) {
						return D-1;
					}
				}
				{
					int cnt0 = 0, cnt1 = 0;
					int cntBoth = 0;
					int cntAll = 0;
					for (int i = 0; i < N; i++) {
						if (i == diam[0] || i == diam[1]) continue;
						bool g0 = dist[0][i] == D-1;
						bool g1 = dist[1][i] == D-1;
						cnt0 += g0;
						cnt1 += g1;
						cntBoth += g0 && g1;
						cntAll += g0 || g1;
					}
					assert(cntBoth + cntAll == cnt0 + cnt1);
					if (cntBoth >= 2) {
						return D-1;
					}
					if (cnt0 >= 2 && cnt1 >= 2 && cntAll >= 4) {
						return D-1;
					}
					return D-2;
				}
			}
		};

		int ans = go();
		cout << ans << '\n';
	}

	return 0;
}
