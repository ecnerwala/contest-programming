#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class SlowestNim {

public:

	vector <int> move(vector <int> piles) {
		int moves = std::accumulate(piles.begin(), piles.end(), 0);
		int b = -1;
		int v = 64;
		for (int i = 0; i < int(piles.size()); ++i) {
			if (piles[i] == 0) continue;
			int cv = __builtin_ctz(piles[i]);
			if (b == -1 || cv < v) {
				b = i;
				v = cv;
			}
		}
		return {moves, b, 1};
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::SlowestNim;

// vim:ft=cpp
