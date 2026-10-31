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

	int N; cin >> N;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	int ans = 0;
	std::y_combinator([&](auto self, int cur, int prv) -> bool {
		bool has_ch = false;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			int nxt_has = self(nxt, cur);
			if (nxt_has) has_ch = true;
		}
		if (has_ch) ans++;
		return !has_ch;
	})(0, -1);

	cout << (N - 2 * ans) + (N - ans) << '\n';

	return 0;
}
