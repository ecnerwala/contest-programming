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
	vector<int64_t> X(N);
	for (auto& x : X) { int64_t a; cin >> a; x = a; }
	for (auto& x : X) { int64_t y; cin >> y; x = y - x; }

	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::y_combinator([&](auto self, int cur) -> void {
		for (int nxt : adj[cur]) {
			adj[nxt].erase(std::find(adj[nxt].begin(), adj[nxt].end(), cur));
			self(nxt);
			X[cur] += X[nxt];
		}
	})(0);
	if (X[0]) {
		cout << "NIE" << '\n';
		exit(0);
	}

	std::y_combinator([&](auto self, int cur) -> void {
		for (int nxt : adj[cur]) {
			X[nxt] = X[cur] - X[nxt];
			self(nxt);
		}
	})(0);
	auto mi = *std::min_element(X.begin(), X.end());
	for (auto& v : X) v -= mi;

	const int64_t MOD = 1e18;
	array<int64_t, 2> ans{0, 0};
	for (auto v : X) {
		ans[0] += v;
		ans[1] += ans[0] / MOD;
		ans[0] %= MOD;
	}

	cout << "TAK" << '\n';
	if (ans[1]) {
		cout << ans[1] << fixed << setfill('0') << setw(18) << ans[0] << '\n';
	} else {
		cout << ans[0] << '\n';
	}

	return 0;
}
