#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class WaitingForBusAgain {

public:

	double expectedBus(vector <int> F) {
		// what's the probability that k of them are smaller than the min
		int N = int(F.size());
		int minF = *min_element(F.begin(), F.end());
		vector<double> dpSmaller(N+1);
		dpSmaller[0] = 1;
		vector<double> evSmaller(N+1);
		for (int v = 0; v < N; v ++) {
			int f = F[v];
			double probSmaller = double(minF) / double(f); // probability that it's smaller
			double probBigger = 1 - probSmaller;
			for (int i = v; i >= 0; i--) {
				double oldProb = dpSmaller[i]; dpSmaller[i] = 0;
				double oldEv = evSmaller[i]; evSmaller[i] = 0;
				dpSmaller[i+1] += oldProb * probSmaller;
				evSmaller[i+1] += oldEv * probSmaller + oldProb * probSmaller * v;

				dpSmaller[i] += oldProb * probBigger;
				evSmaller[i] += oldEv * probBigger;
			}
		}
		double ans = 0;
		for (int i = 1; i <= N; i++) {
			ans += evSmaller[i] / i;
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	mt19937 mt(48);
	vector<int> F;
	for (int i = 0; i < 1000; i++) {
		F.push_back(mt() % int(1e6) + 1);
	}
	cerr << WaitingForBusAgain().expectedBus(F) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::WaitingForBusAgain;

// vim:ft=cpp
