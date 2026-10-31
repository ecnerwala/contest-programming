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
		adj[v].push_back(edge{u, cap, cost});
	}

	vector<pair<priority_queue<pair<int64_t, int64_t>>, int64_t>> dp(N);
	std::y_combinator([&](auto self, int cur, int prv, int64_t cur_cost) -> void {
		for (const auto& e : adj[cur]) {
			int nxt = e.dest;
			if (nxt == prv) continue;
			int64_t nxt_cost = cur_cost + e.cost;
			self(nxt, cur, nxt_cost);
			if (adj[nxt].size() == 1) {
				dp[nxt].first.push({nxt_cost, e.cap});
				dp[nxt].second += e.cap;
			}
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
	})(0, -1, 0);

	vector<pair<int64_t, int64_t>> vals; vals.reserve(dp[0].first.size());
	while (!dp[0].first.empty()) {
		int64_t v, c; tie(v, c) = dp[0].first.top(); dp[0].first.pop();
		vals.push_back({v, c});
	}

	int64_t ans = 0;
	int64_t cur_cost = 0;
	while (!vals.empty()) {
		int64_t v, c; tie(v, c) = vals.back(); vals.pop_back();
		if (v <= 0) {
			ans += c;
			cur_cost += v * c;
		} else {
			assert(v >= 0);
			if (cur_cost + v * c <= 0) {
				ans += c;
				cur_cost += v * c;
			} else {
				ans += (-cur_cost) / v;
				long double x = ((-cur_cost) % v);
				x /= v;
				cout << fixed << setprecision(20) << ((long double)(ans) + x) << '\n';
				exit(0);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
