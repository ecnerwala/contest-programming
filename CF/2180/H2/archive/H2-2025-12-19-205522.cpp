#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (false) {
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
		for (int n = 0; n < 1000; n++) {
			cerr << (n < 2 ? 0 : vals[n-2]) << " \n"[(n+1) % 8 == 0];
		}
		for (int l = 0; l < 10; l++) {
			for (int n = 1; n < 1000; n++) {
				if (__builtin_ctz(n) == l) cerr << (n < 2 ? 0 : vals[n-2]) << ' ';
			}
			cerr << '\n';
		}
		cerr << '\n';
	}

	auto solve_lin = [&](int64_t g_hi) -> int {
		g_hi += 1;
		int ans = 0;
		int cnt = 0;
		for (; g_hi; g_hi /= 4) {
			cnt += g_hi;
			cnt -= g_hi / 2;
		}
		return std::array<int, 4>{0, 2, 3, 1}[cnt & 3];
		return ans;
	};
	auto solve = [&](int64_t g_hi, int64_t d) -> int {
		assert(d >= 1);
		if (d & 1) {
			g_hi = g_hi / d + ((g_hi % d) & 1);
			return solve_lin(g_hi);
		} else {
			if (g_hi & 1) {
				int64_t g = g_hi / d + 1;
				if (!(__builtin_ctzll(g) & 1)) {
					return __builtin_popcountll(g) & 1 ? 1 : 2;
				}
			}
			return 0;
		}
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
