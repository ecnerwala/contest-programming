#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class OptimalMemoryGame {

public:

	int findPairs(string known) {
		int N = int(known.size())/2;
		std::map<char, int> freq;
		for (char c : known) {
			if (c != '-') freq[c]++;
		}
		int n1 = 0;
		int n2 = 0;
		for (auto it : freq) {
			if (it.second == 1) n1++;
			else if (it.second == 2) n2++;
			else assert(false);
		}
		int n0 = N - n1 - n2;

		int ans = 0;
		if (n0 == 0) {
			ans = n1;
		} else if (n0 == 1 && n1 == 0) {
			ans = 1;
		}

		ans += n2;
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::OptimalMemoryGame;

// vim:ft=cpp
