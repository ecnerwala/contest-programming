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

	std::vector<double> R(N);
	std::vector<double> X(N);

	for (int i = 0; i < N; i++) {
		cin >> R[i] >> X[i];
	}

	std::vector<std::vector<std::pair<int, double>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; double f; cin >> u >> v >> f; u--, v--;
		adj[u].push_back({v, f});
		adj[v].push_back({u, f});
	}

	double ans = 0;
	std::y_combinator([&](auto self, int cur, int prv) -> std::map<double, double> {
		for (auto [nxt, f] : adj[cur]) {
			if (nxt == prv) continue;
			X[nxt] = std::max(X[nxt], X[cur] * f);
			self(nxt, cur);
		}
	})(0, -1);

	return 0;
}
