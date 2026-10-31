#include<bits/stdc++.h>

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
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		struct edge {
			int dest;
			int64_t cap;
			int64_t cost;
		};
		vector<vector<edge>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v, cap, cost; cin >> u >> v >> cap >> cost; u--, v--;
			adj[u].push_back(edge{v, cap, cost});
		}

		vector<pair<priority_queue<pair<int64_t, int64_t>>, int64_t>> dp(N);
		std::y_combinator([&](auto self, int cur, int64_t cur_cost) -> void {
			for (const auto& e : adj[cur]) {
				int nxt = e.dest;
				int64_t nxt_cost = cur_cost + e.cost;
				self(nxt, nxt_cost);
				dp[nxt].first.push({nxt_cost, e.cap});
				dp[nxt].second += e.cap;
				while (dp[nxt].second > e.cap) {
					assert(!dp[nxt].first.empty());
					int64_t v, c; tie(v, c) = dp[nxt].first.top(); dp[nxt].first.pop();
					int64_t delta = min(c, dp[nxt].second - e.cap);
					dp[nxt].second -= delta;
					c -= delta;
					if (c > 0) {
						dp[nxt].first.push({v, c});
					}
				}

				if (dp[nxt].first.size() > dp[cur].first.size()) {
					swap(dp[nxt].first, dp[cur].first);
				}
				assert(dp[nxt].first.size() <= dp[cur].first.size());
				while (!dp[nxt].first.empty()) {
					dp[cur].first.push(dp[nxt].first.top());
					dp[nxt].first.pop();
				}

				dp[cur].second += dp[nxt].second;
			}
		})(0, 0);
		int64_t ans = 0;
		while (!dp[0].first.empty()) {
			int64_t v, c; tie(v, c) = dp[0].first.top(); dp[0].first.pop();
			ans += v * c;
		}

		cout << "Case #" << case_num << ": " << dp[0].second << ' ' << ans << '\n';
	}

	return 0;
}
