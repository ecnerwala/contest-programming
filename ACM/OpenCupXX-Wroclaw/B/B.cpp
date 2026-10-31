#include<bits/stdc++.h>
using namespace std;

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
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Z; cin >> Z;
	while (Z--) {
		int N, M; cin >> N >> M;

		vector<int> val(N);

		vector<vector<pair<int, int>>> adj(N);

		vector<bool> ans(M);
		for (int e = 0; e < M; e++) {
			int u, v, w; cin >> u >> v >> w;
			u--, v--;
			if (w) {
				ans[e] = 1;
				val[u] ^= 1;
				val[v] ^= 1;
			} else {
				ans[e] = 0;

				adj[u].emplace_back(v, e);
				adj[v].emplace_back(u, e);
			}
		}

		vector<bool> vis(N);
		auto dfs = std::y_combinator([&](auto self, int cur) -> bool {
			vis[cur] = true;
			for (auto it : adj[cur]) {
				int nxt = it.first;
				int e = it.second;
				if (vis[nxt]) continue;

				if (self(nxt)) {
					ans[e] = true;

					val[nxt] ^= 1;
					val[cur] ^= 1;
				}
			}
			return val[cur];
		});

		bool good = true;
		for (int i = 0; i < N; i++) {
			if (!vis[i]) {
				if (dfs(i)) {
					good = false;
					break;
				}
			}
		}
		if (good) {
			cout << "TAK" << '\n';
			for (int e = 0; e < M; e++) cout << ans[e] << '\n';
		} else {
			cout << "NIE" << '\n';
		}
	}

	return 0;
}
