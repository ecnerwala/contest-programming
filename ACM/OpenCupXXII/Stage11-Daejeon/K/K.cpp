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
		int N, K; int64_t L, R; cin >> N >> K >> L >> R;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int x, y; cin >> x >> y; x--, y--;
			adj[x].push_back(y);
			adj[y].push_back(x);
		}

		using vals_t = std::vector<std::array<int64_t, 2>>;
		auto compress_vals = [&](vals_t& r) -> void {
			std::sort(r.begin(), r.end());
			int len = 0;
			for (int i = 0; i < int(r.size()); i++) {
				if (len > 0 && r[i][0] <= r[len-1][1] + (R-L)) {
					r[len-1][1] = std::max(r[len-1][1], r[i][1]);
				} else {
					r[len++] = r[i];
				}
			}
			r.resize(len);
		};

		auto convolve_dps = [&](std::vector<vals_t> a, std::vector<vals_t> b) -> std::vector<vals_t> {
			std::vector<vals_t> r(a.size() + b.size());
			for (int i = 0; i < int(a.size()); i++) {
				for (int j = 0; j < int(b.size()); j++) {
					if (i+j > K) continue;
					for (auto [alo, ahi] : a[i]) {
						for (auto [blo, bhi] : b[j]) {
							if (alo + blo > R) continue;
							r[i+j].push_back({alo + blo, std::min(ahi + bhi, R)});
						}
					}
				}
			}
			for (auto& v : r) compress_vals(v);
			while (!r.empty() && r.back().empty()) r.pop_back();
			return r;
		};

		std::vector<bool> ans(K+1);
		std::vector<std::vector<vals_t>> dp(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			if (A[cur] <= R) {
				dp[cur].push_back(vals_t({std::array<int64_t, 2>{A[cur], A[cur]}}));
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
				dp[cur] = convolve_dps(std::move(dp[cur]), std::move(dp[nxt]));
			}
			for (int i = int(dp[cur].size())-1; i >= 0; i--) {
				if (!dp[cur][i].empty() && dp[cur][i].back()[1] >= L) {
					if (prv == -1) {
						ans[i] = true;
					} else {
						if (i+1 > K) continue;
						if (i+1 == int(dp[cur].size())) {
							dp[cur].emplace_back();
						}
						if (!dp[cur][i+1].empty() && dp[cur][i+1][0][0] <= R-L) {
							dp[cur][i+1][0][0] = 0;
						} else {
							dp[cur][i+1].insert(dp[cur][i+1].begin(), {0, 0});
						}
					}
				}
			}
		})(0, -1);

		for (int i = 0; i <= K; i++) {
			cout << int(ans[i]);
		}
		cout << '\n';
	}

	return 0;
}
