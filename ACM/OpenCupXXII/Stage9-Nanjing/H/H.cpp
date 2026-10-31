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

	int CASES; cin >> CASES;
	while (CASES--) {
		int N; cin >> N;
		std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
		std::vector<int> T(N); for (auto& t : T) cin >> t;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		const int64_t INF = 1e18;
		auto [ans, _] = std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int64_t, int64_t> {
			if (prv != -1) {
				auto it = std::find(adj[cur].begin(), adj[cur].end(), prv);
				adj[cur].erase(it);
			}

			int64_t val_normal = 0;

			int64_t best_extra = 0;

			int64_t best_extra_3 = 0;
			int64_t best_2touch = -INF;

			int64_t twotouch = A[cur];
			for (int nxt : adj[cur]) {
				auto [norm, two] = self(nxt, cur);
				two -= norm;

				val_normal += norm;
				twotouch += norm;

				best_extra = std::max(best_extra, A[nxt]);
				best_extra = std::max(best_extra, best_extra_3 + two);
				if (T[nxt] == 3 && best_2touch != -INF) {
					best_extra = std::max(best_extra, best_2touch + A[nxt]);
				}

				if (T[nxt] == 3) {
					best_extra_3 = std::max(best_extra_3, A[nxt]);
				}
				best_2touch = std::max(best_2touch, two);
			}

			return {val_normal + best_extra, twotouch};
		})(0, -1);

		ans += A[0];
		cout << ans << '\n';
	}

	return 0;
}
