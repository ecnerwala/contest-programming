#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MOD = int(1e9) + 7;
	int N, D; cin >> N >> D;
	std::vector<std::pair<int, int>> cows(N);
	for (auto& [p, o] : cows) { cin >> p >> o; }

	std::vector<int> p2(N+1);
	p2[0] = 1;
	for (int i = 1; i <= N; i++) { p2[i] = 2 * p2[i-1]; p2[i] -= (p2[i] >= MOD) * MOD; }

	std::vector<int> dp(N+1);
	int ans = 1;

	int cnt_camper = 0;
	int ways_near = 0;
	int ways_far = 1;
	for (int i = N-1, j = N-1; i >= 0; i--) {
		if (cows[i].second == 0) {
			cnt_camper++;
			ways_near *= 2;
			ways_near -= (ways_near >= MOD) * MOD;
		} else {
			for (; cows[j].first > cows[i].first + D; j--) {
				if (cows[j].second == 0) {
					cnt_camper--;
				} else {
					ways_near -= int(int64_t(dp[j]) * int64_t(p2[cnt_camper]) % MOD);
					ways_near += (ways_near < 0) * MOD;

					ways_far += dp[j];
					ways_far -= (ways_far >= MOD) * MOD;
				}
			}
			dp[i] = int((ways_near + int64_t(ways_far) * int64_t(p2[cnt_camper])) % MOD);
			ans += dp[i];
			ans -= (ans >= MOD) * MOD;
		}
	}

	cout << ans << '\n';

	return 0;
}
