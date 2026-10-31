#include<bits/stdc++.h>

template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, D; cin >> N >> D;
	vector<int> par(N);
	par[0] = -1;
	for (int i = 1; i < N; i++) cin >> par[i];

	vector<vector<int>> dp(N, vector<int>({1}));
	for (int ch = N-1; ch > 0; ch--) {
		int pa = par[ch];

		dp[ch].push_back(0); // depth can go up by one

		if (dp[ch].size() > dp[pa].size()) {
			swap(dp[ch], dp[pa]);
		}
		assert(dp[pa].size() >= dp[ch].size());
		// merge ch into pa
		for (int i = 0; i < int(dp[ch].size()); i++) {
			int j = max(i, D-i);
			dp[pa][int(dp[pa].size())-1-i] = max(
				dp[ch][int(dp[ch].size())-1-i] + (j < int(dp[pa].size()) ? dp[pa][int(dp[pa].size())-1-j] : 0),
				(j < int(dp[ch].size()) ? dp[ch][int(dp[ch].size())-1-j] : 0) + dp[pa][int(dp[pa].size())-1-i]
			);
		}
		for (int i = int(dp[ch].size())-1; i > 0; i--) {
			setmax(dp[pa][int(dp[pa].size())-1-(i-1)], dp[pa][int(dp[pa].size())-1-i]);
		}
	}
	cout << dp[0].back() << '\n';

	return 0;
}
