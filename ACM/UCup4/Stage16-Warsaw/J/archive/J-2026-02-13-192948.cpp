#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T_tot, K, T_tgt; cin >> T_tot >> K >> T_tgt;

	std::vector<std::pair<int, int>> evts; evts.reserve(int(2e6));
	for (int k = 0; k < K; k++) {
		int c; cin >> c;
		while (c--) {
			int a, b; cin >> a >> b; a--;
			evts.push_back({a, k});
			evts.push_back({b, k});
		}
	}
	std::sort(evts.begin(), evts.end());
	std::vector<int> tot_time(1 << K);
	{
		int cur_t = 0;
		int cur_msk = 0;
		for (auto [nxt_t, k] : evts) {
			tot_time[cur_msk] += (nxt_t - cur_t);
			cur_t = nxt_t;
			cur_msk ^= (1 << k);
		}
		tot_time[cur_msk] += (T_tot - cur_t);
		cur_t = T_tot;
	}

	std::vector<std::array<int, 2>> dp(1 << K);
	for (int i = 0; i < (1 << K); i++) {
		dp[i] = {tot_time[i], 0};
	}
	for (int k = 0; k < K; k++) {
		for (int i = 0; i < (1 << K); i++) {
			if (i & (1 << k)) continue;
			auto v_0 = dp[i];
			auto v_1 = dp[i | (1 << k)];
			dp[i] = {v_0[0] + v_1[0], v_0[1] + v_1[1]};
			dp[i | (1 << k)] = {v_0[0], v_0[1] + v_1[0]};
		}
	}
	int ans = 0;
	for (int i = 0; i < (1 << K); i++) {
		if (T_tot - (dp[i][0] + dp[i][1]) >= T_tgt) {
			ans++;
		}
	}
	cout << ans << '\n';

	return 0;
}
