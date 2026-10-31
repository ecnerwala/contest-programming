#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; int64_t D; cin >> N >> K >> D;
	vector<int64_t> T(N+2);
	T[0] = 0;
	for (int i = 1; i <= N; i++) cin >> T[i];
	T[N+1] = T[N] + (N+2) * D;

	vector<int64_t> dp(N+2, -1);
	dp[0] = 0;
	for (int i = 0; i <= N; i++) {
		int64_t cur_time = T[i];
		int64_t cur_val = dp[i];
		if (cur_val == -1) continue;

		int cur_cnt = 0;
		int j = i+1;
		cur_time += D;
		while (j <= N+1 && T[j] <= cur_time) {
			cur_cnt++;
			j++;
		}

		while (cur_cnt > 0) {
			int end_cnt = cur_cnt;
			int64_t end_val = cur_val;

			cur_val += min(K, cur_cnt) * cur_time;
			cur_cnt -= min(K, cur_cnt);
			cur_time += D;

			while (j <= N+1 && T[j] <= cur_time) {
				end_cnt++;
				cur_cnt++;
				if (end_cnt <= K) {
					int64_t cnd = end_val + end_cnt * T[j];
					if (dp[j] == -1 || cnd < dp[j]) dp[j] = cnd;
				}
				j++;
			}
		}

		assert(cur_cnt == 0);

		while (j <= N+1 && cur_cnt+1 <= K) {
			cur_cnt++;
			int64_t cnd = cur_val + cur_cnt * T[j];
			if (dp[j] == -1 || cnd < dp[j]) dp[j] = cnd;
			j++;
		}
	}

	int64_t ans = dp[N+1];
	for (int i = 1; i <= N+1; i++) {
		ans -= T[i];
	}
	cout << ans << '\n';

	return 0;
}
