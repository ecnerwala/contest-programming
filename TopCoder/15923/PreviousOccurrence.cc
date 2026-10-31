#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class PreviousOccurrence {

public:

	int testValue(int defaultValue, int cap) {
		unordered_map<int, int> lastOcc;
		int numSmall = 0;
		int cur = 0;
		for (int i = 0; true; i++) {
			int nxt;
			if (lastOcc.count(cur)) {
				nxt = i - lastOcc[cur];
			} else {
				if (cur <= cap) {
					numSmall++;
					if (numSmall == cap+1) {
						cerr << cur << '\n';
						return i;
					}
				}
				nxt = defaultValue;
			}
			lastOcc[cur] = i;

			cur = nxt;
		}
	}

	int findValue(int defaultValue, int query) {
		if (defaultValue == 1) {
			if (query == 0) return 0;
			if (query == 1) return 1;
			return -1;
		}
		unordered_map<int, int> lastOcc;
		int cur = 0;
		for (int i = 0; true; i++) {
			if (cur == query) {
				return i;
			}

			int nxt;
			if (lastOcc.count(cur)) {
				nxt = i - lastOcc[cur];
			} else {
				nxt = defaultValue;
			}
			lastOcc[cur] = i;

			cur = nxt;
		}
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << 1 << ' ' << PreviousOccurrence().testValue(1, 1) << '\n';
	for (int i = 0; i <= 100; i++) {
		if (i == 1) continue;
		cerr << i << ' ' << PreviousOccurrence().testValue(i, 7000) << '\n';
	}
	cerr << 1000 << ' ' << PreviousOccurrence().testValue(1000, 7000) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::PreviousOccurrence;

// vim:ft=cpp
