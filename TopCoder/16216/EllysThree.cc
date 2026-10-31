#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class EllysThree {

public:

	int getMin(int N, int M, vector <string> matrix) {
		const int INF = 1e9;

		std::array<int, 3> dp{0,INF,INF};
		for (int s = 0; s <= N+M-2; s++) {
			std::array<int, 3> cost{0,0,0};
			for (int i = 0; i < N; i++) {
				int j = s - i;
				if (0 <= j && j < M) {
					int d = matrix[i][j] - '0';
					cost[d % 3] += 0;
					cost[(d+1) % 3] += (d == 9 ? 2 : 1);
					cost[(d+2) % 3] += (d == 0 ? 2 : 1);
				}
			}
			std::array<int, 3> ndp{INF,INF,INF};
			for (int v = 0; v < 3; v++) {
				for (int a= 0; a < 3; a++) {
					ndp[(a+v)%3] = min(ndp[(a+v)%3], dp[a] + cost[v]);
				}
			}
			dp = ndp;
		}
		return dp[0];
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::EllysThree;

// vim:ft=cpp
