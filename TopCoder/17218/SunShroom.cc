#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class SunShroom {

public:

	long long produceSun(vector<long long> plantingTime, long long queryTime) {
		long long ans = 0;
		for (int64_t t : plantingTime) {
			t = std::max<int64_t>(queryTime - t, 0);
			t /= 24;
			ans += t * 15 + std::max<int64_t>(t - 4, 0) * 10;
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::SunShroom;

// vim:ft=cpp
