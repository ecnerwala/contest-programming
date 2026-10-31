#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N); P[0] = -1;
		for (int i = 1; i < N; i++) {
			cin >> P[i]; P[i]--;
		}

		std::vector<int> sz(N, 1);
		for (int i = N-1; i > 0; i--) {
			if (i > 0) {
				sz[P[i]] += sz[i];
			}
		}
		std::vector<int> dp(N, 0);
		for (int i = 0; i < N; i++) {
		}
		for (int i = N-1; i >= 0; i--) {
			// gather all its children
			// dp[i] is the maximum child count
			dp[i] = std::max(dp[i], sz[i] & 1);
			if (i > 0) {
				dp[P[i]] = std::max(dp[P[i]], 1 + std::max(0, dp[i] - (sz[P[i]] - 1 - sz[i])));
			}
		}
		cout << (N - dp[0]) / 2 << '\n';
	}

	return 0;
}
