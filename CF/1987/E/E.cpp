#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//int T; cin >> T;
	std::mt19937 mt(48);
	int T = 1000;
	while (T--) {
		//int N; cin >> N;
		int N = std::uniform_int_distribution<int>(1, 1000)(mt);
		std::vector<int> A(N);
		//for (auto& a : A) cin >> a;
		for (auto& a : A) { a = std::uniform_int_distribution<int>(0, 10)(mt); }
		std::vector<int> P(N);
		P[0] = -1;
		//for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }
		for (int i = 1; i < N; i++) { P[i] = std::uniform_int_distribution<int>(0, i-1)(mt); }
		std::vector<std::vector<std::pair<int, int>>> dp(N);
		int64_t ans = 0;
		for (int i = N-1; i >= 0; i--) {
			if (dp[i].empty()) {
				// is leaf is a special case, but we don't need to do anything
			}
			{
				int tot_dist = 0;
				int val = A[i];
				while (val > 0) {
					int delta = dp[i].empty() ? val : std::min(val, dp[i].back().first);
					ans += int64_t(tot_dist) * delta;
					val -= delta;
					if (dp[i].empty()) {
						// noop
						assert(val == 0);
						continue;
					}
					dp[i].back().first -= delta;
					if (dp[i].back().first > 0) {
						assert(val == 0);
						continue;
					}
					assert(dp[i].back().first == 0);
					tot_dist += dp[i].back().second;
					dp[i].pop_back();
				}
				dp[i].push_back({A[i], tot_dist+1});
			}
			assert(!dp[i].empty());
			{
				int p = P[i];
				if (p != -1) {
					if (dp[p].empty()) {
						std::swap(dp[p], dp[i]);
					} else {
						assert(!dp[p].empty());
						assert(!dp[i].empty());
						// merge the two in linear time
						std::vector<std::pair<int, int>> ndp; ndp.reserve(dp[i].size() + dp[p].size());
						int dist_i = 0;
						int dist_p = 0;
						while (true) {
							int cnt = 0;
							if (dist_i == 0) {
								if (dp[i].empty()) break;
								cnt += dp[i].back().first;
								dist_i = dp[i].back().second;
								dp[i].pop_back();
							}
							if (dist_p == 0) {
								if (dp[p].empty()) break;
								cnt += dp[p].back().first;
								dist_p = dp[p].back().second;
								dp[p].pop_back();
							}
							int d = std::min(dist_i, dist_p);
							assert(d > 0);
							ndp.push_back({cnt, d});
							dist_i -= d;
							dist_p -= d;
						}
						std::reverse(ndp.begin(), ndp.end());
						assert(!ndp.empty());
						std::swap(dp[p], ndp);
					}
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
