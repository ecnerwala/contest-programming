#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

double dp[51][51][51][51];

class BlindBoxSets {

public:

	double expectedPurchases(int numSets, int numItems) {
		for (int d = 0; d <= numItems; d++) {
			for (int c = 0; c+d <= numItems; c++) {
				for (int b = 0; b+c+d <= numItems; b++) {
					for (int a = 0; a+b+c+d <= numItems; a++) {
						int s = a + b + c + d;
						if (s == 0) {
							// we're done
							dp[a][b][c][d] = 0;
						} else {
							double others = (a ? a * dp[a-1][b][c][d] : 0) + (b ? b * dp[a+1][b-1][c][d] : 0) + (c ? c * dp[a][b+1][c-1][d] : 0) + (d ? d * dp[a][b][c+1][d-1] : 0);
							// dp[a][b][c][d] * numItems = numItems + others + (numItems - s) * dp[a][b][c][d]
							dp[a][b][c][d] = (numItems + others) / s;
						}
					}
				}
			}
		}

		if (numSets == 1) {
			return dp[numItems][0][0][0];
		} else if (numSets == 2) {
			return dp[0][numItems][0][0];
		} else if (numSets == 3) {
			return dp[0][0][numItems][0];
		} else if (numSets == 4) {
			return dp[0][0][0][numItems];
		} else assert(false);
	}
};

void my_tests() {
	// FILL IN TESTS
	auto slow = [&](int numSets, int numItems) {
		mt19937 mt(48);
		auto dist = uniform_int_distribution<int>(0, numItems-1);
		double sum = 0;
		const int trials = 10000;
		for (int z = 0; z < trials; z++) {
			int res = 0;

			vector<int> cnt(numItems, numSets);
			int tot = numItems * numSets;
			while (tot) {
				res++;
				int i = dist(mt);
				if (cnt[i]) cnt[i]--, tot--;
			}

			sum += res;
		}
		return sum / trials;
	};

	for (int numSets = 1; numSets <= 4; numSets++) {
		for (int numItems : {1, 2, 3, 4, 5, 9, 10, 11, 12, 20, 30, 40, 45, 49, 50}) {
			cerr << numSets << ' ' << numItems << ' ' << slow(numSets, numItems) << ' ' << BlindBoxSets().expectedPurchases(numSets, numItems) << '\n';
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::BlindBoxSets;

// vim:ft=cpp
