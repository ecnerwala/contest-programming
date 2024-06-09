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
		assert(N >= 2);
		std::vector<std::pair<int, bool>> dp_down(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			bool can_use = true;
			int cnt = 0;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				auto [ch_cnt, ch_used] = dp_down[nxt];
				cnt += ch_cnt;
				if (ch_used) can_use = false;
			}
			if (can_use) cnt++;
			dp_down[cur] = {cnt, can_use};
		})(0, -1);
		int ans = dp_down[0].first;
		if (adj[0].size() == 1) {
			ans = std::max(ans, dp_down[adj[0][0]].first + 1);
		}

		std::vector<std::pair<int, bool>> dp_up(N);
		dp_up[0] = {0, false};
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			int tot_cnt, tot_used;
			std::tie(tot_cnt, tot_used) = dp_up[cur];
			if (prv != -1 && int(adj[cur].size()) == 1) {
				ans = std::max(ans, tot_cnt + 1);
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				auto [ch_cnt, ch_used] = dp_down[nxt];
				tot_cnt += ch_cnt;
				tot_used += ch_used;
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				auto [ch_cnt, ch_used] = dp_down[nxt];
				tot_cnt -= ch_cnt;
				tot_used -= ch_used;
				{
					bool can_use = tot_used == 0;
					dp_up[nxt] = {tot_cnt + can_use, can_use};
				}
				tot_cnt += ch_cnt;
				tot_used += ch_used;
			}

			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(0, -1);
		cout << ans << '\n';
	}

	return 0;
}
