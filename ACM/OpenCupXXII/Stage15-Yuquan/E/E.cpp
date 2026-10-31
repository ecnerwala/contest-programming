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

constexpr bool TESTING = true;
int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, E; cin >> N >> K >> E;
	int64_t cutoff = int64_t(E) * K;
	std::vector<int> V(N);
	std::vector<int> A(N);
	std::vector<int> B(N);
	for (int i = 0; i < N; i++) {
		cin >> V[i] >> A[i] >> B[i];
	}

	auto apply_update = [&](std::vector<int>& dp, int a, int v) {
		for (int i = K; i >= a; i--) {
			dp[i] = std::max(dp[i], dp[i-a] + v);
		}
	};

	auto apply_A = [&](std::vector<int>& dp, int i) {
		if (i == N) return;
		apply_update(dp, A[i], V[i]);
	};
	auto apply_B = [&](std::vector<int>& dp, int i) {
		if (i == N) return;
		apply_update(dp, B[i], V[i]);
	};

	auto is_wide = [&](const std::vector<int>& dp) {
		return std::accumulate(dp.begin(), dp.end(), int64_t(0)) <= cutoff;
	};

	auto is_interval_wide = [&](int l, int r) {
		cerr << "compute " << l << ' ' << r << '\n';
		if (r > N || l < 0) return true;
		if (l > r) return false;
		auto dp = std::vector<int>(K+1);
		for (int i = 0; i < l; i++) {
			apply_A(dp, i);
		}
		for (int i = l; i < r; i++) {
			apply_B(dp, i);
		}
		for (int i = r; i < N; i++) {
			apply_A(dp, i);
		}
		return is_wide(dp);
	};

	int64_t ans = 0;
	std::y_combinator([&](auto self, std::vector<int> dp, int l_lo, int l_hi, int r_lo, int r_hi) -> void {
		cerr << "run dp" << ' ' << l_lo << '-' << l_hi << ' ' << r_lo << '-' << r_hi << '\n';
		assert(l_lo < l_hi);
		if (TESTING) {
			auto dp2 = std::vector<int>(K+1);
			for (int i = 0; i < l_lo; i++) {
				apply_A(dp2, i);
			}
			for (int i = l_hi; i < r_lo; i++) {
				apply_B(dp2, i);
			}
			for (int i = r_hi; i < N; i++) {
				apply_A(dp2, i);
			}
			assert(dp2 == dp);
		}
		int l_md = l_lo + (l_hi - l_lo) / 2;
		auto cdp = dp;
		for (int i = l_lo; i < l_md; i++) {
			apply_A(cdp, i);
		}
		for (int i = l_md; i < std::min(l_hi, r_lo); i++) {
			apply_B(cdp, i);
		}
		int r_mi = std::max(l_md, r_lo) - 1;
		int r_ma = r_hi;
		if (TESTING) {
			assert(is_interval_wide(l_md, r_ma));
			assert(!is_interval_wide(l_md, r_mi));
			assert(l_md - 1 <= r_mi && r_mi <= N);
		}
		while (r_ma - r_mi > 1) {
			int r_md = (r_mi + r_ma) / 2;
			auto c2dp = cdp;
			for (int i = r_mi + 1; i < r_md; i++) {
				apply_B(c2dp, i);
			}
			for (int i = r_md; i < r_ma; i++) {
				apply_A(c2dp, i);
			}
			if (is_wide(c2dp)) {
				for (int i = r_md; i < r_ma; i++) {
					apply_A(cdp, i);
				}
				r_ma = r_md;
			} else {
				for (int i = r_mi + 1; i < r_md; i++) {
					apply_B(cdp, i);
				}
				r_mi = r_md;
			}
		}
		if (TESTING) {
			assert(is_interval_wide(l_md, r_ma));
			assert(!is_interval_wide(l_md, r_mi));
			assert(l_md - 1 <= r_mi && r_mi <= N);
		}
		// is_wide(l_md, r_ma) = true
		// is_wide(l_md, r_mi) = false
		//cerr << l_md << ' ' << r_mi << '/' << r_ma << '\n';
		ans += N - std::max(l_md, r_mi);

		if (l_lo < l_md) {
			auto ndp = dp;
			for (int i = l_md; i < std::min(l_hi, r_lo); i++) {
				apply_B(ndp, i);
			}
			for (int i = r_ma; i < r_hi; i++) {
				apply_A(ndp, i);
			}
			self(ndp, l_lo, l_md, r_lo, r_ma);
		}

		if (l_md+1 < l_hi) {
			auto ndp = dp;
			for (int i = l_lo; i < l_md + 1; i++) {
				apply_A(ndp, i);
			}
			for (int i = std::max(r_mi, l_hi); i < r_lo; i++) {
				apply_B(ndp, i);
			}
			self(ndp, l_md + 1, l_hi, r_mi, r_hi);
		}

	})(std::vector<int>(K+1, 0), 0, N, 0, N+1);

	cout << ans << '\n';

	return 0;
}
