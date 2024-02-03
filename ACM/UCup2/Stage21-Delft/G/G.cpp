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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string W; cin >> W;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	if (int(W.size()) == 1) {
		cout << N << '\n';
		exit(0);
	} else if (int(W.size()) == 2) {
		if (W[0] == W[1]) {
			cout << 2 * (N-1) << '\n';
		} else {
			cout << N-1 << '\n';
		}
		exit(0);
	} else assert(int(W.size()) == 3);

	if (N == 1) {
		cout << 0 << '\n';
		exit(0);
	}

	// 0 is middle, 1 is not
	std::vector<std::array<std::array<int64_t, 2>, 2>> dp(N, {std::array<int64_t, 2>{0, 0}, {0, 0}});
	std::vector<int64_t> diffs; diffs.reserve(N);
	auto dfs = std::y_combinator([&](auto self, int cur, int prv) -> void {
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
		}
		for (int our_val = 0; our_val < 2; our_val++) {
			int64_t all_mid_value = 0;
			diffs.clear();
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				diffs.push_back(dp[nxt][our_val][1] - dp[nxt][our_val][0]);
				all_mid_value += dp[nxt][our_val][0];
			}
			std::sort(diffs.begin(), diffs.end(), std::greater<>());
			assert(int(diffs.size()) == int(adj[cur].size()) - 1);
			int64_t cur_val = all_mid_value;
			for (int ch_cnt = 0; ch_cnt <= int(diffs.size()); ch_cnt++) {
				if (ch_cnt > 0) cur_val += diffs[ch_cnt-1];

				for (int par_val = 0; par_val < 2; par_val++) {
					int64_t outer_cnt = ch_cnt + par_val;
					int64_t mid_cnt = int(adj[cur].size()) - ch_cnt;
					int64_t cnd_val = cur_val;
					if (our_val == 0) {
						// Few cases to analyze
						if (W[0] == W[1] && W[1] == W[2]) {
							// aaa
							cnd_val += int64_t(mid_cnt) * int64_t(mid_cnt - 1);
						} else if (W[0] == W[1] || W[1] == W[2]) {
							// aab
							cnd_val += int64_t(mid_cnt) * int64_t(outer_cnt);
						} else if (W[0] == W[2]) {
							// bab
							cnd_val += int64_t(outer_cnt) * int64_t(outer_cnt - 1);
						} else {
							// bac
							cnd_val += int64_t(outer_cnt / 2) * int64_t((outer_cnt + 1) / 2);
						}
					}
					setmax(dp[cur][par_val][our_val], cnd_val);
				}
			}
		}
	});

	int r0 = 0, r1 = adj[0][0];
	dfs(r0, r1);
	dfs(r1, r0);
	int64_t ans = 0;
	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
			setmax(ans, dp[r0][a][b] + dp[r1][b][a]);
		}
	}
	cout << ans << '\n';

	return 0;
}
