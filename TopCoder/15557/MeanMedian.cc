#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class MeanMedian {

public:

	int minEffort(int needMean, int needMedian, vector <int> d) {
		int N = int(d.size());
		sort(d.begin(), d.end());
		int needSum = max(0, needMean * N - (needMedian * (N/2 + 1)));
		int ans = 0;
		for (int i = 0; i <= N/2; i++) {
			int v = min(needSum, 10 - needMedian) + needMedian;
			needSum -= (v - needMedian);
			ans += v * d[i];
		}
		for (int i = N/2 + 1; i < N; i++) {
			int v = min(needSum, 10);
			needSum -= v;
			ans += v * d[i];
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MeanMedian;

// vim:ft=cpp
