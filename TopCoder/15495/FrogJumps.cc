#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class FrogJumps {

public:

	int canMeet(long long x1, long long y1, long long x2, long long y2, long long k1, long long k2) {
		long long g = __gcd(k1, k2);
		return (x1 - x2) % g == 0 && (y1 - y2) % g == 0;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FrogJumps;

// vim:ft=cpp
