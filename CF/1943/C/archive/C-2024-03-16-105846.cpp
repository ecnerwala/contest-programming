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
		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> diam; diam.reserve(N);
		{
			std::vector<int> par(N);
			auto dfs = std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int, int> {
				par[cur] = prv;
				std::pair<int, int> res(0, cur);
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					auto r = self(nxt, cur);
					r.first++;
					res = std::max(res, r);
				}
				return res;
			});
			int v = dfs(dfs(0, -1).second, -1).second;
			while (v != -1) {
				diam.push_back(v);
				v = par[v];
			}
		}

		std::vector<std::pair<int, int>> ops; ops.reserve(N);
		int L = int(diam.size());
		if (L & 1) {
			int v = diam[L / 2];
			for (int d = 0; d <= L/2; d++) {
				ops.push_back({v, d});
			}
		} else {
			for (int v : {diam[L/2-1], diam[L/2]}) {
				for (int d = 1; d <= L/2; d += 2) {
					ops.push_back({v, d});
				}
			}
		}

		cout << ops.size() << '\n';
		for (auto [v, d] : ops) {
			cout << v+1 << ' ' << d << '\n';
		}
	}

	return 0;
}
