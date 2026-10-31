#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<int64_t> A(N); for (auto& a : A) cin >> a;

		int ans = 3;

		vector<int> dp(N);
		vector<int> dp_skip(N);
		dp[0] = 1;
		dp_skip[0] = 1;

		for (int i = 1; i < N; i++) {
			if (i >= 2 && A[i] - A[i-1] == A[i-1] - A[i-2]) {
				dp[i] = dp[i-1] + 1;
				dp_skip[i] = dp_skip[i-1] + 1;
			} else {
				dp[i] = 2;
			}
			if (i >= 3 && A[i] - A[i-2] == 2 * (A[i-2] - A[i-3])) {
				ans = max(ans, dp[i-2] + 2);
			}
			if (i >= 3 && 2 * (A[i] - A[i-1]) == A[i-1] - A[i-3]) {
				dp_skip[i] = max(dp_skip[i], 4);
				if (i >= 4 && A[i] - A[i-1] == A[i-3] - A[i-4]) {
					dp_skip[i] = max(dp_skip[i], 3 + dp[i-3]);
				}
			}
			ans = max(ans, dp[i]+1);
			ans = max(ans, dp_skip[i]);
		}

		ans = min(ans, N);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
