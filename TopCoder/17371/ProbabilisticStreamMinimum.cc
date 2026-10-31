#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ProbabilisticStreamMinimum {

public:

	double calculate(int N, int K) {
		double ans = 1;
		for (int i = 0; i < K; i++) {
			ans *= K*K-i;
			ans /= K*K;
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::ProbabilisticStreamMinimum;

// vim:ft=cpp
