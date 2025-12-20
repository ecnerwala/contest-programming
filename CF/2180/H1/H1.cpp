#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [&](int64_t g_hi, int64_t d) -> int {
		assert(d >= 1);
		int l = __builtin_ctzll(d);
		d >>= l;
		g_hi = g_hi / d + ((g_hi % d) & 1);
		int ans = 0;
		ans ^= ((g_hi >> (l+1)) & 1);
		g_hi &= (int64_t(2) << l) - 1;
		if (g_hi == 0) return ans;
		ans ^= 1;
		if (g_hi & 1) {
			if ((g_hi >> l) & 1) {
				int i = 63 - __builtin_clzll((int64_t(2) << l) - g_hi);
				ans ^= ((l^i) & 1) ? 2 : 1;
			} else {
				int i = 63 - __builtin_clzll(g_hi);
				ans ^= ((l^i) & 1) ? 0 : 1;
			}
		}
		return ans;
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
