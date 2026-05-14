#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		std::string S; std::cin >> S;
		int N = int(S.size());

		std::vector<std::array<int64_t, 3>> dp(N+1);
		dp[0].fill(0);
		for (int i = 1; i <= N; i++) {
			for (int st = 0; st < 3; st++) {
				dp[i][st] = 1e18;
				for (int en = 0; en < 3; en++) {
					if (en == st) continue;
					int64_t tot_cost = 0;
					int cur_cost = 0;
					int cur_val = 0+1+2 - st - en;
					for (int j = i-1; j >= 0; j--) {
						int c = S[j] - 'A';
						if (c != en) {
							if (cur_val != c) { cur_val = c, cur_cost++; }
							tot_cost += cur_cost;
						}
						dp[i][st] = std::min(dp[i][st], (N-i) + dp[j][en] + tot_cost);
					}
				}
				if (S[i-1] - 'A' == st) dp[i][st] = dp[i-1][st];
			}
		}

		std::cout << dp[N][0] << '\n';
	}

	return 0;
}
