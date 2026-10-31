#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<int> H(N); for (auto& h : H) cin >> h;

	vector<int> dp(N);
	dp[0] = 0;
	for (int i = 1; i < N; i++) {
		int res = int(1.1e9);
		for (int j = max(0, i-K); j < i; j++) {
			res = min(res, dp[j] + abs(H[j] - H[i]));
		}
		dp[i] = res;
	}
	cout << dp[N-1] << '\n';

	return 0;
}
