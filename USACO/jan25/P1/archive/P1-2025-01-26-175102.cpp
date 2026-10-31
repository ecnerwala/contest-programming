#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<int>> weight(N, std::vector<int>(N));
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			int w; cin >> w;
			weight[i][j] = weight[j][i] = w;
			// we'll take it out for now, and add it back if we want it
			if (w < 0) ans -= w;
		}
	}

	std::vector<std::vector<int>> dp(N, std::vector<int>(N, -1));
	for (int i = N-1; i >= 0; i--) {
		dp[i][i] = 0;
		for (int j = i+1; j < N; j++) {
			int cur_cost = dp[i][j-1];
			for (int k = j; k < N; k++) {
				cur_cost += k == j ? weight[i][k] : std::min(weight[i][k], 0);
				int cnd = cur_cost + dp[j][k];
				if (j == i+1 || cnd < dp[i][k]) dp[i][k] = cnd;
			}
		}
	}
	cout << ans + dp[0][N-1] << '\n';

	return 0;
}
