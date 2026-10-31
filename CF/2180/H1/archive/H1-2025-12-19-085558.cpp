#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve_log = [&](int64_t g_hi, int l) -> int {
		int ans = 0;
		int64_t whole = g_hi >> (l+1);
		ans ^= (whole & 1);
		g_hi &= (int64_t(2) << l) - 1;
		auto add_range = [&](int v, int i) -> void {
			int64_t cnt = std::min(g_hi, int64_t(1) << i);
			g_hi -= cnt;
			if (cnt & 1) ans ^= v;
		};
		add_range((l&1) ? 1 : 0, 0);
		for (int i = 0; i < l; i++) {
			add_range(((l ^ i) & 1) ? 0 : 1, i);
		}
		for (int i = l-1; i >= 1; i--) {
			add_range(((l ^ i) & 1) ? 2 : 1, i);
		}
		add_range((l&1) ? 1 : 2, 1);
		assert(g_hi == 0);
		return ans;
	};

	if (false) {
		const int L = 20;
		const int N = 8000;
		std::vector<std::vector<int>> vals(N+1, std::vector<int>(L+1));
		for (int n = 0; n <= N; n++) {
			for (int l = 0; l <= L; l++) {
				std::array<bool, 4> has{false, false, false, false};
				if (l > 0) {
					has[vals[n][l-1]] = true;
				}
				if (n >= (1 << l)) {
					has[vals[n-(1<<l)][l]] = true;
				}
				if (n >= (2 << l)) {
					has[vals[n-(2<<l)][l+1]] = true;
				}
				int v = 0;
				while (has[v]) v++;
				vals[n][l] = v;
			}
		}
		for (int l = 0; l <= 8; l++) {
			for (int n = 0; n < (2 << l); n++) {
				assert(vals[n+(2<<l)][l] == vals[n][l]);
				cerr << vals[n][l] << ' ';
			}
			cerr << '\n';
		}
		for (int l = 0; l <= L; l++) {
			int res = 0;
			for (int n = 0; n <= N; n++) {
				assert(res == solve_log(n, l));
				res ^= vals[n][l];
			}
		}
	}

	auto solve_log = [&](int64_t g_hi, int l) -> int {
		int ans = 0;
		int64_t whole = g_hi >> (l+1);
		ans ^= (whole & 1);
		g_hi &= (int64_t(2) << l) - 1;
		auto add_range = [&](int v, int i) -> void {
			int64_t cnt = std::min(g_hi, int64_t(1) << i);
			g_hi -= cnt;
			if (cnt & 1) ans ^= v;
		};
		add_range((l&1) ? 1 : 0, 0);
		for (int i = 0; i < l; i++) {
			add_range(((l ^ i) & 1) ? 0 : 1, i);
		}
		for (int i = l-1; i >= 1; i--) {
			add_range(((l ^ i) & 1) ? 2 : 1, i);
		}
		add_range((l&1) ? 1 : 2, 1);
		assert(g_hi == 0);
		return ans;
	};

	auto solve = [&](int64_t g_hi, int64_t d) -> int {
		assert(d >= 1);
		int l = __builtin_ctzll(d);
		d >>= l;
		int64_t cnt_hi = g_hi % d;
		int64_t cnt_lo = d - cnt_hi;
		int res = 0;
		if (cnt_lo & 1) res ^= solve_log(g_hi / d, l);
		if (cnt_hi & 1) res ^= solve_log(g_hi / d + 1, l);
		return res;
	};

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			int64_t a, b, c, l, r; cin >> a >> b >> c >> l >> r;
			assert((c - b) == (b - a));
			int64_t g_hi = r - c;
			int64_t g_lo = l - c;
			assert(g_hi >= g_lo);
			ans ^= solve(g_hi+1, c-b) ^ solve(g_lo, c-b);
		}
		cout << (ans ? "Bug" : "Feature") << '\n';
	}

	return 0;
}
