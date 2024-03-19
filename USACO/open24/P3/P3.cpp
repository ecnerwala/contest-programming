#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) {
	if (b < a) a = b;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L, R, N_pts, K; cin >> L >> R >> N_pts >> K;
	assert(L % R == 0);
	std::vector<int> A(N_pts);
	for (auto& a : A) cin >> a;
	std::sort(A.begin(), A.end());

	// We walk K units in 1 time, while robots walk 1 unit in 1 time

	const int INF = 2 * L + 1;

	int ans = INF;
	std::vector<int> dp(R << (R-1), INF);
	dp[1/2 * R + 0] = 0;
	for (int m = 1; m < (1 << R); m += 2) {
		for (int i = 0; i < R; i++) {
			if (!(m & (1 << i))) continue;
			int t0 = dp[m / 2 * R + i];
			if (t0 == INF) continue;
			if (i == 0) {
				assert(m == 1);
				assert(t0 == 0);
			} else {
				int pos = (t0 + i * (L / R)) % L;
				auto it = std::lower_bound(A.begin(), A.end(), pos);
				int npos = it == A.end() ? (L + *A.begin()) : *it;
				t0 += npos - pos;
			}
			if (m == (1 << R) - 1) {
				setmin(ans, t0);
				continue;
			}
			for (int j = 0; j < R; j++) {
				if (m & (1 << j)) continue;

				auto ceil_div = [](int a, int b) -> int { return (a + b - 1) / b; };
				// Then, we can either go forwards for (j-i+R)%R * (L/R) / (K-1) or backwards for (i-j+R)%R * (L/R) / (K+1)
				int delta_t = ceil_div((i-j+R)%R * (L / R), K+1);
				if (K > 1) {
					setmin(delta_t, ceil_div((j-i+R)%R * (L / R), K-1));
				}
				int t = t0 + delta_t;
				int nm = m | (1 << j);
				setmin(dp[nm / 2 * R + j], t);
			}
		}
	}

	cout << int64_t(ans) * K << '\n';

	return 0;
}
