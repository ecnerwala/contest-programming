#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, P; cin >> N >> M >> P;
	vector<int64_t> msk(N);
	vector<int> cnt(M);
	for (auto& a : msk) {
		string s; cin >> s;
		for (int i = 0; i < M; i++) {
			cnt[i] += (s[i] - '0');
			a += (int64_t(s[i] - '0')) << i;
		}
	}

	int64_t all_msk = 0;
	for (int i = 0; i < M; i++) {
		if (cnt[i] * 2 >= N) {
			all_msk |= int64_t(1) << i;
		}
	}
	for (auto& a : msk) {
		a &= all_msk;
	}

	sort(msk.begin(), msk.end());

	vector<pair<int64_t, int>> dp(N);
	for (int i = 0; i < N; i++) {
		if (!dp.empty() && dp.back().first == msk[i]) dp.back().second++;
		else dp.push_back({msk[i], 1});
	}

	int ans = 0;
	int64_t best_msk = 0;
	for (auto [m, v] : dp) {
		if (v*2 >= N && __builtin_popcountll(m) > ans) ans = __builtin_popcountll(m), best_msk = m;
	}

	for (int b = 0; b < M; b++) {
		if (!(all_msk & (int64_t(1) << b))) continue;
		int64_t pb = int64_t(1) << b;
		vector<pair<int64_t, int>> ndp; ndp.reserve(dp.size()*2);
		for (int i = 0, j = 0; i < int(dp.size()) || j < int(dp.size()); ) {
			if (i < int(dp.size()) && !(dp[i].first & pb)) {
				i++;
				continue;
			}
			if (i < int(dp.size()) && j < int(dp.size()) && (dp[i].first ^ pb) == dp[j].first) {
				ndp.push_back({dp[j].first, dp[i].second + dp[j].second});
				i++, j++;
			} else if (i < int(dp.size()) && (j == int(dp.size()) || (dp[i].first ^ pb) < dp[j].first)) {
				ndp.push_back({dp[i].first ^ pb, dp[i].second});
				i++;
			} else {
				ndp.push_back(dp[j]);
				j++;
			}
		}
		dp = std::move(ndp);
		for (auto [m, v] : dp) {
			if (v*2 >= N && __builtin_popcountll(m) > ans) ans = __builtin_popcountll(m), best_msk = m;
		}
	}

	for (int b = 0; b < M; b++) {
		cout << ((best_msk >> b) & 1);
	}
	cout << '\n';

	return 0;
}
