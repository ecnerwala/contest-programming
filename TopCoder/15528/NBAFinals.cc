#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class NBAFinals {

public:

	int dubsAgain(vector <int> scores, string team) {
		int v = 0;
		for (int i = 0; i < int(scores.size()); i++) {
			if (team[i] == 'R') {
				if (scores[i] == 0) scores[i] = 1;
				v -= scores[i];
			} else {
				if (scores[i] == 0) scores[i] = 4;
				v += scores[i];
			}
		}
		return v > 0;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::NBAFinals;

// vim:ft=cpp
