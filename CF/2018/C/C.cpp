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
		std::vector<int> pref(N+1);
		[&](this auto& self, int cur, int prv, int d) -> int {
			int max_depth = d;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				max_depth = std::max(max_depth, self(nxt, cur, d+1));
			}
			// keep this guy in [depth, max_depth]
			pref[d]++;
			pref[max_depth+1]--;
			return max_depth;
		}(0, -1, 0);
		for (int i = 0; i < N; i++) pref[i+1] += pref[i];
		cout << N - *std::max_element(pref.begin(), pref.end()) << '\n';
	}

	return 0;
}
