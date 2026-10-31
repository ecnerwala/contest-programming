#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;

	vector<int> H(N); for (auto& h : H) cin >> h;

	vector<int> dp(N);
	dp[0] = 0;
	assert(N >= 2);
	dp[1] = abs(H[1] - H[0]);
	for (int i = 2; i < N; i++) {
		dp[i] = min(dp[i-1] + abs(H[i-1] - H[i]), dp[i-2] + abs(H[i-2] - H[i]));
	}

	cout << dp[N-1] << '\n';

	return 0;
}
