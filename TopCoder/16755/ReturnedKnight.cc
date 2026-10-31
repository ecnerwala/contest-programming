#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ReturnedKnight {

public:

	vector <string> construct(int P, int Q) {
		{ int g = __gcd(P, Q); P /= g, Q /= g; }
		if (P < 2 * Q) return {};
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::ReturnedKnight;

// vim:ft=cpp
//
// ...
// ..N
// ..#
// ...
