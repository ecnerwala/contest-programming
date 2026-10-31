#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S; cin >> S;
	int M = int(S.size());
	vector<int64_t> dp(M);
	int64_t ans = 0;
	for (int z = 0; z < N; z++) {
		for (int x = M-1; x > 0; x--) {
			dp[x] = dp[x-1];
		}
		dp[0] = 1;
		for (int x = 1; x < M; x++) {
			dp[x] += dp[x-1];
		}
		ans += dp[M-1];
	}
	cout << ans << '\n';

	return 0;
}
