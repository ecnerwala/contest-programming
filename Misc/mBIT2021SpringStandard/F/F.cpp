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
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	vector<vector<array<int, 2>>> dels(N+1);
	std::y_combinator([&](auto self, int cur, int prv) -> int {
		int resp = 0;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			resp = max(resp, self(nxt, cur));
		}
		if (prv != -1) {
			dels[resp].push_back({prv, cur});
		}
		return resp;
	})(0, -1);
	for (int d = 0; d <= N; d++) {
		for (auto [x, y] : dels[d]) {
			cout << x+1 << ' ' << y+1 << '\n';
		}
	}
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			cout << nxt+1 << ' ' << cur+1 << '\n';
			self(nxt, cur);
		}
	})(0, -1);

	return 0;
}
