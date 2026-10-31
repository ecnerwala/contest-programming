#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class NearPalindromesDiv1 {

public:

	int solve(string S) {
		int N = int(S.size());
		int msk_all = 0;
		int msk = 0;
		for (char c : S) {
			msk_all |= (1 << (c - 'a'));
			msk ^= (1 << (c - 'a'));
		}

		auto solve_msk = [&](int a) -> int {
			assert(__builtin_popcount(a) % 2 == 0);
			int res = 0;
			while (a) {
				int i = __builtin_ctz(a);
				a &= a-1;
				int j = __builtin_ctz(a);
				a &= a-1;
				res += abs(i-j);
			}
			return min(res, 26-res);
		};

		int ans = 1e9;
		if (N & 1) {
			for (int i = 0; i < 26; i++) {
				ans = min(ans, solve_msk(msk ^ (1 << i)));
			}
			return ans;
		} else {
			return solve_msk(msk);
		}
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::NearPalindromesDiv1;

// vim:ft=cpp
