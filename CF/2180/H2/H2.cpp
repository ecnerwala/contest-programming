#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (true) {
		const int N = 8000;
		std::vector<int> vals(N+1);
		for (int n = 0; n <= N; n++) {
			std::array<bool, 4> has{false, false, false, false};
			if (n >= 1) {
				has[vals[n-1]] = true;
			}
			if (n >= 2) {
				has[vals[(n-2)/2]] = true;
			}
			int v = 0;
			while (has[v]) v++;
			vals[n] = v;
		}
		for (int n = 0; n < 100; n++) {
			cerr << vals[n] << " \n"[(n+1) % 16 == 0];
		}
		cerr << '\n';
	}

	auto solve_log = [&](int64_t g_hi) -> int {
		return 0;
	};
	auto solve = [&](int64_t g_hi, int64_t d) -> int {
		assert(d >= 1);
		int64_t cnt_hi = g_hi % d;
		int64_t cnt_lo = d - cnt_hi;
		int res = 0;
		if (cnt_lo & 1) res ^= solve_log(g_hi / d);
		if (cnt_hi & 1) res ^= solve_log(g_hi / d + 1);
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
