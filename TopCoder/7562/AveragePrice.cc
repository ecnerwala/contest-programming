#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class AveragePrice {

public:

	double nonDuplicatedAverage(vector <int> prices) {
		sort(prices.begin(), prices.end());
		prices.resize(unique(prices.begin(), prices.end()) - prices.begin());
		double ans = 0;
		for (int i : prices) ans += i;
		return ans / int(prices.size());
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::AveragePrice;

// vim:ft=cpp
