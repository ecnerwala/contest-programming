#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	struct day_t {
		int no_v, no_c;
		int yes_v, yes_c;
		int extra_c, extra_p;
	};
	std::vector<day_t> days(N);
	for (auto& d : days) {
		cin >> d.no_v >> d.no_c >> d.yes_v >> d.yes_c >> d.extra_c >> d.extra_p;
	}

	std::vector<std::array<double, 4>> dp(M+1, {0, 0, 0, 0});
	std::vector<std::array<double, 4>> ndp(M+1, {0, 0, 0, 0});
	for (int z = N-1; z >= 0; z--) {
		auto d = days[z];
		for (int c = 0; c <= M; c++) {
			for (int m = 0; m < 4; m++) {
				double v = -1;
				if (d.no_c <= c && dp[c - d.no_c][m] >= -0.5) {
					v = std::max(v, dp[c - d.no_c][m] + d.no_v);
				}
				if (m == 3) {
					if (d.yes_c + d.extra_c <= c && dp[c - (d.yes_c + d.extra_c)][0] >= -0.5) {
						v = std::max(v, dp[c - (d.yes_c + d.extra_c)][m] + d.yes_v);
					}
				} else {
					if (d.yes_c <= c) {
						bool good = true;
						double val = 0;
						if (d.extra_p > 0) {
							double nv = dp[c - d.yes_c][m | 1];
							if (nv < -0.5) good = false;
							else val += double(d.extra_p) * 0.01 * nv;
						}
						if (d.extra_p < 100) {
							double nv = dp[c - d.yes_c][m | 2];
							if (nv < -0.5) good = false;
							else val += double(100 - d.extra_p) * 0.01 * nv;
						}
						if (good) v = std::max(v, val + d.yes_v);
					}
				}
				ndp[c][m] = v;
			}
		}
		std::swap(dp, ndp);
	}
	if (dp[M][0] < -0.5) cout << -1 << '\n';
	else cout << fixed << setprecision(12) << dp[M][0] << '\n';

	return 0;
}
