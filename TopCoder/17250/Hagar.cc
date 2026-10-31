#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Hagar {

public:

	double expectedProfit(vector <int> gold) {
		std::sort(gold.begin(), gold.end());
		std::reverse(gold.begin(), gold.end());
		double sum = 0;
		double ans = 0;
		for (int i = 0; i < int(gold.size()) && gold[i] > 0 && gold[i] > ans; i++) {
			sum += 1. / gold[i];
			ans = i / sum;
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::Hagar;

// vim:ft=cpp
