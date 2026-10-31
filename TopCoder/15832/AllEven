#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using st = array<long long, 1024>;

class AllEven {

public:

	st countRec(long long lo, long long hi) {
		assert(0 < lo && lo < hi);
		st res;
		for (int m = 0; m < 1024; m++) res[m] = 0;
		while (hi > lo && hi % 10) {
			int m = 0;
			for (long long v = --hi; v; v /= 10) {
				m ^= (1 << (v % 10));
			}
			res[m]++;
		}
		while (lo < hi && lo % 10) {
			int m = 0;
			for (long long v = lo++; v; v /= 10) {
				m ^= (1 << (v % 10));
			}
			res[m]++;
		}
		if (lo < hi) {
			assert(lo % 10 == 0 && hi % 10 == 0);
			st a = countRec(lo / 10, hi / 10);
			for (int m = 0; m < 1024; m++) {
				for (int d = 0; d <= 9; d++) {
					res[m ^ (1 << d)] += a[m];
				}
			}
		}
		return res;
	}

	long long countInRange(long long lo, long long hi) {
		if (lo == 0) lo++;
		if (lo > hi) return 0;
		return countRec(lo, hi + 1)[0];
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::AllEven;

// vim:ft=cpp
