#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<pair<int, int>> trees(N);
		for (int i = 0; i < N; i++) {
			cin >> trees[i].first >> trees[i].second;
		}
		sort(trees.begin(), trees.end());

		unordered_map<int, int> dp;
		for (auto [p, h] : trees) {
			dp[p+h] = max(dp[p+h], dp[p] + h);
			dp[p] = max(dp[p], dp[p-h] + h);
		}
		int ans = 0;
		for (auto [_, l] : dp) {
			ans = max(ans, l);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
