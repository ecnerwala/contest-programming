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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N); for (auto& a : A) cin >> a;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		const int64_t INF = 2e18;
		std::vector<int> sz(N);
		std::vector<std::vector<int64_t>> out_dp(N);
		std::vector<std::vector<int64_t>> in_dp(N);
		std::y_combinator([&](auto&& self, int cur, int prv) -> std::pair<std::vector<int>, std::vector<int>> {
			sz[cur] = 1;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				sz[cur] += sz[nxt];
			}
			out_dp[cur].assign(sz[cur] + 1, INF);
			in_dp[cur].assign(N+1, INF);
			for (int tot = 1; tot <= N; tot++) {
				std::vector<int64_t> dp; dp.reserve(sz[cur]+1);
				std::vector<int64_t> ndp; ndp.reserve(sz[cur]+1);
				dp.assign(2, INF);
				dp[1] = tot * A[cur];
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					ndp.assign(dp.size() + sz[nxt], INF);
					for (int i = 1; i < int(dp.size()); i++) {
						setmin(ndp[i], dp[i] + in_dp[nxt][tot]);
						for (int j = 1; j <= sz[nxt]; j++) {
							setmin(ndp[i+j], dp[i] + out_dp[nxt][j]);
						}
					}
					std::swap(dp, ndp);
				}
				// if we're going out, we're just fine
				if (tot <= sz[cur]) {
					setmin(out_dp[cur][tot], dp[tot]);
				}
				for (int i = 1; i <= tot && i <= sz[cur]; i++) {
					setmin(in_dp[cur][tot - i], dp[i]);
				}
			}
		})(0, -1);
		cout << in_dp[0][0] << '\n';
	}

	return 0;
}
