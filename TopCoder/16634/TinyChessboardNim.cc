#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using state = array<int, 4>;

bool is_winning(state s) {
	int a = s[0], b = s[1], c = s[2], d = s[3];
	if (a != c && b != d) return true;
	if (a == c && b == d) return false;
	if (a == c) swap(a,b), swap(c,d);
	assert(b == d);
	if (a > c) swap(a, c);
	assert(a < c && b == d);

	int delta = c - a;
	int bd = b / delta;
	int ad = a / delta;
	return (ad < bd);
}

class TinyChessboardNim {

public:

	int countWinningMoves(vector <int> rice) {
		assert(int(rice.size()) == 4);
		state S{rice[0], rice[1], rice[3], rice[2]};
		int ans = 0;
		// winning move only if it makes two of them equal
		for (int fx = 0; fx < 2; fx++, swap(S[0], S[2])) {
			for (int fy = 0; fy < 2; fy++, swap(S[1], S[3])) {
				// we'll decrement S[0] and S[1];
				for (int d : set<int>({S[0] - S[2], S[1] - S[3]})) {
					if (d <= 0 || d > min(S[0], S[1])) continue;
					state ns = S;
					ns[0] -= d, ns[1] -= d;
					ans += !is_winning(ns);
				}
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::TinyChessboardNim;

// vim:ft=cpp
