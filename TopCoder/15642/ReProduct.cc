#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int MAXV = 1.1e7;
const int V = 1e7;
struct {
	int prod;
	int val;
	int dest;
} vals[MAXV];

void precomp() {
	for (int i = 0; i <= 9; i++) {
		vals[i].prod = i;
		vals[i].val = 0;
		vals[i].dest = i;
	}

	for (int i = 10; i <= V; i++) {
		vals[i].prod = vals[i/10].prod * (i%10);
		vals[i].val = vals[vals[i].prod].val + 1;
		vals[i].dest = vals[vals[i].prod].dest;
		if (vals[i].val >= 8) {
			cerr << i << ' ' << vals[i].val << ' ' << vals[i].dest << '\n';
		}
	}
}

class ReProduct {

public:

	long long minimize(vector <int> base, int goal) {
		precomp();
		return 0;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::ReProduct;

// vim:ft=cpp
