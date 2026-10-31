#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class KnapsackTweak {

public:

	int smallest(vector <int> items, int target) {
		int N = int(items.size());
		std::sort(items.begin(), items.end());

		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) pref[i+1] = pref[i] + items[i];

		std::vector<int> best(N+1);
		for (int i = 0; i <= N; i++) best[i] = std::abs(pref[i] - target);

		const int V = int(2e5) + 10;
		std::vector<int64_t> dp(V+1);
		dp[0] = 1;

		for (int i = N; i >= 0; i--) {
			if (i < N) {
				int d = items[i];
				for (int v = V; v >= d; v--) {
					dp[v] |= dp[v-d] << 1;
				}
			}

			{
				for (int v = 0; v <= V; v++) {
					if (!dp[v]) continue;
					int v_cnt = 63 - __builtin_clzll(dp[v]);
					int cur_v = v + pref[i];
					int cur_cnt = v_cnt + i;
					best[cur_cnt] = min(best[cur_cnt], std::abs(cur_v - target));
				}
			}
		}
		int ans = best[1];
		for (int i = 1; i <= N; i++) {
			ans = std::min(ans, (best[i] + i-1) / i);
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::KnapsackTweak;

// vim:ft=cpp
