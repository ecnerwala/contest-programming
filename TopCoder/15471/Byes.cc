#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Byes {

	int numZeroes(long long v) {
		if (v == 0) return 0;
		return 64 - __builtin_clzll(v) - __builtin_popcountll(v);
	}

	int numByesSlow(long long v) {
		if (v == 1) return 0;
		int res = int(v & 1) + numByesSlow((v + 1) / 2);
		assert(res == numZeroes(v - 1));
		return res;
	}

public:
	long long slow(long long lowerBound, int numberOfByes) {
		//while (numZeroes(lowerBound-1) != numberOfByes) lowerBound++;
		while (numByesSlow(lowerBound) != numberOfByes) lowerBound++;
		return lowerBound;
	}

	long long getNumberOfPlayers(long long lowerBound, int numberOfByes) {
		if (numberOfByes > 0) {
			lowerBound = max(lowerBound, 2ll);
		}
		lowerBound --;
		while (numZeroes(lowerBound) < numberOfByes) {
			lowerBound += (lowerBound & (-lowerBound));
		}
		for (long long v = 1; numZeroes(lowerBound) > numberOfByes; v <<= 1) {
			lowerBound |= v;
		}
		return lowerBound+1;
	}
};

void my_tests() {
	for (long long lb = 1; lb <= 1000; lb++) {
		for (int byes = 0; byes <= 60; byes++) {
			cerr << lb << ' ' << byes << '\n';
			Byes().getNumberOfPlayers(lb, byes);
		}
	}
	for (long long lb = 1; lb <= 1000; lb++) {
		for (int byes = 0; byes <= 10; byes++) {
			long long slowval = Byes().slow(lb, byes);
			long long myval = Byes().getNumberOfPlayers(lb, byes);
			cerr << lb << ' ' << byes << ' ' << slowval << ' ' << myval << '\n';
			assert(slowval == myval);
		}
	}
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::Byes;

// vim:ft=cpp
