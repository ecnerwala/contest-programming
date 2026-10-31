#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class WeighCoins {

public:

	int solve(int N, vector <string> L, string result, vector <string> R) {
		auto char_to_int = [&](char c) -> int {
			if ('A' <= c && c <= 'Z') return c - 'A';
			else if ('a' <= c && c <= 'z') return (c - 'a') + 26;
			else if ('0' <= c && c <= '9') return (c - '0') + 52;
			else assert(false);
		};
		int tgt = 0;
		std::vector<int> can_heavier(N);
		std::vector<int> can_lighter(N);
		for (int z = 0; z < int(result.size()); z++) {
			if (result[z] == '=') {
				for (char c : L[z]) {
					int v = char_to_int(c);
					can_heavier[v] --;
					can_lighter[v] --;
				}
				for (char c : R[z]) {
					int v = char_to_int(c);
					can_heavier[v] --;
					can_lighter[v] --;
				}
			} else {
				if (result[z] == '>') {
					std::swap(L[z], R[z]);
					result[z] = '<';
				}
				tgt ++;
				assert(result[z] == '<');
				for (char c : L[z]) {
					int v = char_to_int(c);
					can_lighter[v] ++;
				}
				for (char c : R[z]) {
					int v = char_to_int(c);
					can_heavier[v] ++;
				}
			}
		}
		int num_cnds = 0;
		for (int i = 0; i < N; i++) {
			num_cnds += (can_heavier[i] == tgt);
			num_cnds += (can_lighter[i] == tgt);
		}

		// At any point, we have several possible states:
		// If we've ever seen a !=, then we have disjoint set of things which
		// can be heavier and set of things which can be lighter; the rest must
		// all be good. Thus, our state is just # heavier, # lighter, # known good
		// If we've only seen =, then some things are known good, some things
		// are known bad, and we're not sure which.

		auto ceil_log_3 = [&](int v) -> int {
			int res = 0;
			while (v > 1) {
				v = (v + 2) / 3;
				res++;
			}
			return res;
		};

		// for the heavier/lighter case, you can always go to ceil(num_cnds/3)
		// say you have 3a + x, 3b + y
		// if x = 0 and y = 0 or 1, then weigh a & b vs a & b (extra a & b+y), you get to a+b or a+b+y
		// if x = 0 and y = 2, then weigh a & b+1 vs a & b+1 (extra a & b), you get a+b+1 or a+b
		// if x = 1 and y = 1, then weigh a+1 & b vs a+1 & b (extra a-1 & b+1), you get a+b+1 or a+b
		//    (we need a >= 1 or b >= 1; otherwise, weigh one against something good)
		// if x = 1 and y = 2, then weigh a & b+1 vs a & b+1 (extra a+1 and b), you get a+b+1
		// if x = 2 and y = 2, then weigh a+1 & b+1 vs a+1 & b+1 (extra a & b), you get a+b+2 or a+b (which is optimal still)

		// Otherwise, we have some number of eithers; we put some number of them on the balance pans
		// We either go to #on heavier/lighter, #on lighter/heavier, or #off either.
		// The only constraint on #on is that if N == #either, then #on has to
		// be even (if #either < N, we can pick an extra to balance it).
		// Thus, we can see that we can still usually attain ceil(num_cnds / 3);
		// If N = # either (we haven't weighed anything), then we do have to take an extra weighing sometimes.
		if (num_cnds == 2*N) {
			num_cnds += 2;
		}

		return ceil_log_3(num_cnds);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::WeighCoins;

// vim:ft=cpp
