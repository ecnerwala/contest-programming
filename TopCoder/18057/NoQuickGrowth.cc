#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class NoQuickGrowth {

public:

	int solve_vec(std::vector<int> A) {
		int N = int(A.size());
		for (int i = 0; i < N; i++) {
			A[i] -= 10 * i;
		}
		std::vector<int> dp; dp.reserve(N+1);
		for (int i = 0; i < N; i++) {
			int mi = -1;
			int ma = int(dp.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (A[i] <= dp[md]) {
					mi = md;
				} else {
					ma = md;
				}
			}
			if (ma == int(dp.size())) dp.push_back(A[i]);
			else dp[ma] = A[i];
		}
		return N - int(dp.size());
	}

	int solve(int N, int delta, int spread, int seed) {
		std::vector<int> A(N);
		int64_t state = seed;
		for (int i = 0; i < N; i++) {
			state = (state * 1103515245 + 12345) % (int64_t(1) << 31);
			int diff = int(state % (2*spread + 1));
			A[i] = i*delta + diff - spread;
		}
		return solve_vec(std::move(A));
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::NoQuickGrowth;

// vim:ft=cpp
