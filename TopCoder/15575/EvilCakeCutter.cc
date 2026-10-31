#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class EvilCakeCutter {

public:

	double successProbability(int w, int h, int w1, int h1) {
		ll w2 = w - w1, h2 = h - h1;
		double wans = w2 == 0 ? 1 : double(max(min(w2, 2 * w1 - w2), 0ll)) / double(w2);
		double hans = h2 == 0 ? 1 : double(max(min(h2, 2 * h1 - h2), 0ll)) / double(h2);
		//double ans = 1. - double(max(2 * w1 - w2, 0ll)) * double(max(2 * h1 - h2, 0ll)) / double(w2) / double(h2);
		return 1 - wans * hans;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << EvilCakeCutter().successProbability(1,1,1,1) << '\n';
	cerr << EvilCakeCutter().successProbability(3,3,2,2) << '\n';
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j <= i; j++) {
			for (int k = 0; k < 30; k++) {
				for (int l = 0; l <= k; l++) {
					double v = EvilCakeCutter().successProbability(i, k, j, l);
					assert(0 <= v && v <= 1);
				}
			}
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::EvilCakeCutter;

// vim:ft=cpp
