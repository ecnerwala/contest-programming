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
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back({v, e});
			adj[v].push_back({u, e});
		}

		int ans = 1;
		std::y_combinator([&](auto self, int cur, int prvE, int depth) -> void {
			ans = std::max(ans, depth);
			for (auto [nxt, nxtE] : adj[cur]) {
				if (nxtE == prvE) continue;
				self(nxt, nxtE, depth + (nxtE < prvE));
			}
		})(0, -1, 1);
		cout << ans << '\n';
	}

	return 0;
}
