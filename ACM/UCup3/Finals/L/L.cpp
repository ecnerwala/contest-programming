#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		std::string S; std::cin >> S;
		int N = int(S.size());
		std::reverse(S.begin(), S.end());

		std::vector<std::array<int64_t, 3>> dp(N+1);
		dp[N].fill(0);
		for (int i = N-1; i >= 0; i--) {
			for (int st = 0; st < 3; st++) {
				dp[i][st] = 1e18;
				for (int en = 0; en < 3; en++) {
					if (en == st) continue;
					int64_t tot_cost = 0;
					int cur_cost = 0;
					int cur_val = 0+1+2 - st - en;
					for (int j = i; j < N; j++) {
						int c = S[j] - 'A';
						if (c != en) {
							if (cur_val != c) { cur_val = c, cur_cost++; }
							tot_cost += cur_cost;
						}
						dp[i][st] = std::min(dp[i][st], (N-i) + dp[j+1][en] + tot_cost);
					}
				}
				if (S[i] - 'A' == st) dp[i][st] = dp[i+1][st];
			}
		}

		std::cout << dp[0][0] << '\n';
	}

	return 0;
}
