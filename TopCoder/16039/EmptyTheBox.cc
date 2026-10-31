#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class EmptyTheBox {

public:

	double minExpectedPenalty(int D, int T) {
		int totPenalty = T * (T+1) / 2;

		T = min(T, 2*D);
		vector<int> sum(1<<T);
		for (int i = 0; i < T; i++) {
			for (int m = 0; m < (1<<i); m++) {
				sum[m | (1<<i)] = sum[m] + i+1;
			}
		}

		vector<double> dp(1<<T, 0);
		for (int m = 0; m < (1<<T); m++) {
			vector<double> best(2*D+1, 0);
			for (int v = m; v; v = (v-1) & m) {
				if (sum[v] <= 2 * D) {
					best[sum[v]] = max(best[sum[v]], sum[v] + dp[m^v]);
				}
			}
			for (int d = 2; d <= 2 * D; d++) {
				dp[m] += best[d] * (D - abs(D+1 - d)) / (D * D);
			}
		}

		return totPenalty - dp[(1<<T)-1];
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::EmptyTheBox;

// vim:ft=cpp
