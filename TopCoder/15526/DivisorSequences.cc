#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class DivisorSequences {

	map<int, int> mem;
	int dp(int N) {
		if (mem.count(N)) return mem[N];
		if (N <= 1) return 0;
		int ans = 1;
		for (int i = 2; i <= N / i; i++) {
			if (N % i == 0) {
				ans = max(ans, 1 + dp(i - 1));
				ans = max(ans, 1 + dp(N / i - 1));
			}
		}
		return mem[N] = ans;
	}
public:

	int longest(int N) {
		return max(dp(N-1) + 1, dp(N));
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << DivisorSequences().longest(int(2e9)) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::DivisorSequences;

// vim:ft=cpp
