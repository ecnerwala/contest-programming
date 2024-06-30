#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }
		const int64_t INF = 1e18;
		// is leaf is a special case
		std::vector<std::vector<int64_t>> dp(N);
		int64_t ans = 0;
		for (int i = N-1; i >= 0; i--) {
			if (dp[i].empty()) {
				dp[i].push_back(INF);
			}
			int64_t val = A[i];
			for (int v = 0; val > 0; v++) {
				int64_t delta = std::min(dp[i][v], val);
				ans += delta * v;
				dp[i][v] -= delta;
				val -= delta;
			}
			dp[i][0] += A[i];
			// put the inf back
			dp[i].back() = INF;
			int p = P[i];
			if (p != -1) {
				if (dp[p].empty()) {
					std::swap(dp[p], dp[i]);
				} else {
					int l = std::min(int(dp[p].size()), int(dp[i].size()));
					dp[p].resize(l);
					for (int v = 0; v < l; v++) {
						dp[p][v] += dp[i][v];
					}
					assert(dp[p].back() >= INF);
					// put the inf back
					dp[p].back() = INF;
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
