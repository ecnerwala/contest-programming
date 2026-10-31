#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }

		std::vector<std::array<int, 2>> dp(N, std::array<int, 2>{0, 0});
		for (int i = N-1; i >= 0; i--) {
			int v = std::max(dp[i][0], dp[i][1] + 1);
			if (i == 0) {
				cout << v << '\n';
			} else {
				if (v > dp[P[i]][0]) {
					dp[P[i]][1] = dp[P[i]][0];
					dp[P[i]][0] = v;
				} else if (v > dp[P[i]][1]) {
					dp[P[i]][1] = v;
				}
			}
		}
	}

	return 0;
}
