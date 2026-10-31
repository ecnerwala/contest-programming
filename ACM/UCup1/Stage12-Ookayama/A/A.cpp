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
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<std::array<int, 2>> dp(N, {0, 0});
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		if (adj[cur].size() == (prv != -1)) {
			dp[cur][A[cur] ? 0 : 1]++;
			return;
		}
		dp[cur] = {0, -2*N};
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			dp[cur] = {
				std::max(dp[cur][0] + dp[nxt][0], dp[cur][1] + dp[nxt][1]),
				std::max(dp[cur][0] + dp[nxt][1], dp[cur][1] + dp[nxt][0]),
			};
		}
		dp[cur][A[cur] ? 0 : 1]++;
	})(0, -1);
	cout << std::max(dp[0][0], dp[0][1]) << '\n';

	return 0;
}
