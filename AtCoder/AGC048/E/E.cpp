#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, T; cin >> N >> K >> T;

	vector<vector<int>> B(N, vector<int>(K));
	for (auto& b : B) {
		for (auto& v : b) cin >> v;
		sort(b.begin(), b.end());
	}

	const int MOD = int(1e9)+7;

	std::vector<int> dp(N);
	for (int ti = 0; ti < N; ti++) {
		int ans = 0;
		for (int t : B[ti]) {
			dp[0] = 1;
			for (int si = ti-1; si >= 0; si--) {
				dp[ti-si] = 0;
				int sj = K;
				for (int k = ti-si-1; k >= 0; k--) {
					while (sj > 0 && B[si][sj-1] >= t+T*(k+1)) sj--;

					dp[k+1] = (dp[k+1] + dp[k] * int64_t(sj)) % MOD;
					dp[k] = (dp[k] * int64_t(K - sj)) % MOD;
				}
			}
			for (int k = 0; k <= ti; k++) {
				ans = (ans + dp[k] * int64_t(k)) % MOD;
			}
		}
		for (int a = ti+1; a < N; a++) ans = ans * int64_t(K) % MOD;
		cout << ans << '\n';
	}

	return 0;
}
