#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

	int64_t ans = 0;
	std::vector<int> sz(N, 1);
	std::vector<std::array<int, 2>> dp(N, std::array<int, 2>{0, 0});
	for (int i = N-1; i >= 0; i--) {
		ans += sz[i];
		ans -= dp[i][0] + dp[i][1];
		int v = sz[i] - dp[i][1];
		assert(0 <= v && v <= sz[i]);
		assert(dp[i].empty() || v >= dp[i].back());
		dp[i][0] = v;
		if (P[i] != -1) {
			sz[P[i]] += sz[i];
			if (dp[i][0] >= dp[P[i]][0]) {
				dp[P[i]][1] = std::max(dp[P[i]][0], dp[i][1]);
				dp[P[i]][0] = dp[i][0];
			} else {
				dp[P[i]][1] = std::max(dp[P[i]][1], dp[i][0]);
			}
		}
	}

	ans *= 2;
	cout << ans << '\n';

	return 0;
}
