#include<bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K, M; cin >> N >> K >> M;
	vector<vector<pair<int, int>>> constraints(N);
	for (int i = 0; i < M; i++) {
		int l, r, x; cin >> l >> r >> x; l--, r--;
		constraints[r].emplace_back(l, x);
	}
	int64_t totAns = 1;
	vector<pair<int, int>> dp(N+1);
	for (int k = 0; k < K; k++) {
		int lo,hi;
		int totDp = (dp[lo = hi = 0] = {-1,1}).second;
		for (int i = 0; i < N; i++) {
			totDp += (dp[++hi] = {i,totDp}).second;
			if (totDp >= MOD) totDp -= MOD;
			for (auto it : constraints[i]) {
				if (it.second & (1<<k)) {
					while (lo <= hi && dp[hi].first >= it.first) {
						totDp -= dp[hi--].second;
						if (totDp < 0) totDp += MOD;
					}
				} else {
					while (lo <= hi && dp[lo].first < it.first) {
						totDp -= dp[lo++].second;
						if (totDp < 0) totDp += MOD;
					}
				}
			}
		}
		totAns *= totDp;
		totAns %= MOD;
	}
	cout << totAns << '\n';

	return 0;
}
