#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using state = array<int, 4>;
state canon(state s) {
	if (s[0] > s[2]) swap(s[0], s[2]);
	if (s[1] > s[3]) swap(s[1], s[3]);
	if (s[0] > s[1] || (s[0] == s[1] && s[2] > s[3])) {
		swap(s[0], s[1]);
		swap(s[2], s[3]);
	}
	return s;
}

map<state, bool> mem;
bool is_winning(state s) {
	s = canon(s);
	if (mem.count(s)) {
		return mem[s];
	}
	for (int a = 0; a < 4; a++) {
		for (int b = a+1; b < 4; b += 2) {
			for (int x = 1; x <= min(s[a], s[b]); x++) {
				state ns = s; ns[a] -= x, ns[b] -= x;
				if (!is_winning(ns)) return mem[s] = true;
			}
		}
	}
	return mem[s] = false;
}

bool fast_is_winning(state s) {
	s = canon(s);
	int a = s[0], b = s[1], c = s[2], d = s[3];
	assert(a <= c && b <= d);
	if (a != c && b != d) return true;
	if (a == c && b == d) return false;
	if (a == c) swap(a,b), swap(c,d);
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
				set<int> moves({S[0] - S[2], S[1] - S[3]});
				for (int d : moves) {
					state ns = S;
					if (d <= 0) continue;
					assert(d > 0);
					if (ns[0] < d || ns[1] < d) continue;
					ns[0] -= d, ns[1] -= d;
					ans += !fast_is_winning(ns);
				}
			}
		}
		return ans;
	}
};

void my_tests() {
	for (int a = 0; a < 30; a++) {
		for (int c = 0; c < 30; c++) {
			for (int b = 0; b < 30; b++) {
				for (int d = 0; d < 30; d++) {
					bool w = is_winning(state{a,b,c,d});
					assert(fast_is_winning(state{a,b,c,d}) == w);
				}
			}
		}
	}
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::TinyChessboardNim;

// vim:ft=cpp
