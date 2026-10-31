#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int MW; cin >> MW;
	int W0;
	vector<vector<int>> dp(N+1, vector<int>(3*N+1));
	for (int i = 0; i < N; i++) {
		int w, v; cin >> w >> v;
		if (i == 0) W0 = w;
		assert(W0 <= w && w <= W0 + 3);
		w -= W0;
		for (int c = i; c >= 0; c--) {
			for (int t = 0; t <= 3 * i; t++) {
				dp[c+1][t+w] = max(dp[c+1][t+w], dp[c][t] + v);
			}
		}
	}
	int ans = 0;
	for (int c = 0; c <= N; c++) {
		for (int t = 0; t <= 3 * N; t++) {
			if (1ll * W0 * c + t <= MW) {
				ans = max(ans, dp[c][t]);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
