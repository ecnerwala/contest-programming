#ifndef ECNERWALA_MACHINE
#define NDEBUG
#include <numeric>
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class UniformingArray {

public:

	int minOperations(vector <int> A, int K) {
		int N = int(A.size());
		for (int i = 0; i < N; i++) A[i] = !A[i];

		std::vector<int> pref(N+1);
		pref[0] = 0; for (int i = 0; i < N; i++) { pref[i+1] = pref[i] + A[i]; }

		int max_bad = K/2;

		bool is_good = false;
		for (int i = 0; i+K <= N; i++) {
			if (pref[i+K] - pref[i] <= max_bad) {
				is_good = true;
			}
		}
		if (!is_good) return -1;
		vector<int> dp(N+1, N+1);
		dp[0] = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] == 0) {
				dp[i+1] = min(dp[i+1], dp[i]);
			}

			if (i+K <= N && pref[i+K] - pref[i] <= max_bad) {
				int l = i, r = i+K;
				int best_cost = dp[l] + 1;
				for (int cost = 2; true; cost++) {
					int nl = l;
					while (nl-1 >= 0 && l - (nl-1) <= K && pref[l] - pref[nl-1] <= max_bad) {
						nl--;
					}
					if (nl == l) break;
					best_cost = min(best_cost, dp[nl] + cost);
					l = nl;
				}
				for (int j = i; j <= i+K; j++) {
					dp[j] = min(dp[j], best_cost);
				}
				while (true) {
					best_cost++;
					int nr = r;
					while (nr+1 <= N && (nr+1) - r <= K && pref[nr+1] - pref[r] <= max_bad) {
						nr++;
					}
					if (nr == r) break;
					for (int j = r; j <= nr; j++) {
						dp[j] = min(dp[j], best_cost);
					}
					r = nr;
				}
			}
		}
		if (dp[N] > N) return -1;
		return dp[N];
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::UniformingArray;

// vim:ft=cpp
