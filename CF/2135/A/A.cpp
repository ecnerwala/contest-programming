#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::vector<int>> inds(N+1);
		for (int i = 0; i < N; i++) inds[A[i]].push_back(i);
		std::vector<int> dp(N+1);
		for (int i = N-1; i >= 0; i--) {
			dp[i] = std::max(dp[i], dp[i+1]);
			if (int(inds[A[i]].size()) >= A[i]) {
				int j = inds[A[i]][int(inds[A[i]].size()) - A[i]];
				dp[j] = std::max(dp[j], dp[i+1] + A[i]);
			}
			assert(inds[A[i]].back() == i);
			inds[A[i]].pop_back();
		}
		cout << dp[0] << '\n';
	}

	return 0;
}
