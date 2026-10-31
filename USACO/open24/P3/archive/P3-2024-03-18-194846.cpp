#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L, R, N_pts, K; cin >> L >> R >> N_pts >> K;
	assert(L % R == 0);
	std::vector<int> A(N_pts);
	for (auto& a : A) cin >> a;
	std::sort(A.begin(), A.end());

	// We walk K units in 1 time, while robots walk 1 unit in 1 time

	const int64_t INF = 6 * int64_t(L);

	int64_t ans = INF;
	std::vector<int64_t> dp(R << R, INF);
	dp[1 * R + 0] = 0;
	for (int m = 1; m < (1 << R); m += 2) {
		for (int i = 0; i < R; i++) {
			if (!(m & (1 << i))) continue;
			int64_t t0 = dp[m * R + i];
			if (t0 == INF) continue;
			if (i == 0) {
				assert(m == 1);
				assert(t0 == 0);
			} else {
				int64_t pos = (t0 + i * (L / R)) % L;
				auto it = std::lower_bound(A.begin(), A.end(), pos);
				int64_t npos = it == A.end() ? (L + *A.begin()) : *it;
				t0 += npos - pos;
			}
			cerr << m << ' ' << i << ' ' << t0 << '\n';
			if (m == (1 << R) - 1) {
				ans = std::min(ans, t0);
				continue;
			}
			for (int j = 0; j < R; j++) {
				if (m & (1 << j)) continue;

				auto ceil_div = [](int a, int b) -> int { return (a + b - 1) / b; };
				// Then, we can either go forwards for (j-i+R)%R * (L/R) / (K-1) or backwards for (i-j+R)%R * (L/R) / (K+1)
				int delta_t = ceil_div((i-j+R)%R * (L / R), K+1);
				if (K > 1) {
					delta_t = std::min(delta_t, ceil_div((j-i+R)%R * (L / R), K-1));
				}
				int64_t t = t0 + delta_t;
				int nm = m | (1 << j);
				dp[nm * R + j] = std::min(dp[nm * R + j], t);
			}
		}
	}

	ans *= K;
	cout << ans << '\n';

	return 0;
}
