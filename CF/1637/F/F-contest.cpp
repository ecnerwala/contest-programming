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
	std::vector<int> H(N);
	for (auto& h : H) cin >> h;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<std::pair<int64_t, int>> dp_up(N);
	std::vector<std::pair<int64_t, int>> dp_down(N);

	std::y_combinator([&](auto self, int cur, int prv) -> void {
		int64_t cost = 0;
		int ch_max = 0;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			cost += dp_down[nxt].first;
			ch_max = std::max(ch_max, dp_down[nxt].second);
		}
		if (H[cur] > ch_max) {
			cost += H[cur] - ch_max;
			ch_max = H[cur];
		}
		dp_down[cur] = {cost, ch_max};
	})(0, -1);

	dp_up[0] = {0, 0};

	int64_t MX = *std::max_element(H.begin(), H.end());
	int64_t ans = int64_t(MX) * int64_t(N);

	std::y_combinator([&](auto self, int cur, int prv) -> void {
		int ch_max_1 = 0;
		int ch_max_2 = 0;
		int64_t cost = 0;
		for (int nxt : adj[cur]) {
			auto ch = (nxt == prv) ? dp_up[cur] : dp_down[nxt];
			cost += ch.first;
			if (ch.second > ch_max_1) {
				ch_max_2 = ch_max_1;
				ch_max_1 = ch.second;
			} else if (ch.second > ch_max_2) {
				ch_max_2 = ch.second;
			}
		}
		ans = std::min(ans, MX + cost + std::max(0, H[cur] - ch_max_1));;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto ch = dp_down[nxt];
			auto cur_cost = cost - ch.first;
			auto cur_max = ch_max_1 == ch.second ? ch_max_2 : ch_max_1;
			if (H[cur] > cur_max) {
				cur_cost += H[cur] - cur_max;
				cur_max = H[cur];
			}
			dp_up[nxt] = {cur_cost, cur_max};
			self(nxt, cur);
		}
	})(0, -1);

	cout << ans << '\n';

	return 0;
}
