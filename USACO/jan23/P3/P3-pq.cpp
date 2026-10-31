#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

	std::vector<int> sz(N, 1);
	for (int i = N-1; i > 0; i--) {
		sz[P[i]] += sz[i];
	}

	int64_t ans = 0;
	std::vector<std::priority_queue<int>> dp(N);
	for (int i = N-1; i >= 0; i--) {
		int b0 = 0;
		if (!dp[i].empty()) {
			b0 = dp[i].top();
			dp[i].pop();
		}
		int b1 = 0;
		if (!dp[i].empty()) {
			b1 = dp[i].top();
		}
		ans += b0 + b1;
		int v = sz[i] - b1;
		assert(0 <= v && v <= sz[i]);
		assert(dp[i].empty() || v >= dp[i].top());
		dp[i].push(v);
		if (P[i] != -1) {
			if (dp[i].size() > dp[P[i]].size()) {
				std::swap(dp[i], dp[P[i]]);
			}
			while (!dp[i].empty()) {
				dp[P[i]].push(dp[i].top());
				dp[i].pop();
			}
		}
	}

	ans = std::accumulate(sz.begin(), sz.end(), int64_t(0)) - ans;
	ans *= 2;
	cout << ans << '\n';

	return 0;
}
