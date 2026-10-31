#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

int64_t comp(std::vector<std::string> A) {
	int N = int(A.size());
	int M = int(A[0].size());
	std::vector<std::vector<int64_t>> dp(N, std::vector<int64_t>(M, 0));
	assert(A[0][0] == '-');
	dp[0][0] = 1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (A[i][j] == '*') continue;
			else if (A[i][j] == '-') {
				for (int z = 1; z <= 3; z++) {
					if (i >= z) dp[i][j] += dp[i-z][j];
					if (j >= z) dp[i][j] += dp[i][j-z];
				}
			} else assert(false);
		}
	}
	assert(A[N-1][M-1] == '-');
	return dp[N-1][M-1];
}


class StepHopJumpConstruct {

public:

	vector <string> construct(int W) {
		//cerr << "go" << W << '\n';
		const int N = 49;
		std::vector<std::string> res(N, std::string(N, '*'));
		for (int i = 0; i <= N-1; i += 2) {
			res[N-1][i] = '-';
			res[i][N-1] = '-';
		}
		res[0][0] = '-';
		int x = 0;
		int y = 0;
		for (int l = 0; W; l++) {
			if (l > 0) {
				if (l & 1) {
					// go down
					x++;
					res[x][y] = '-';
					x++;
					res[x][y] = '-';
				} else {
					y++;
					res[x][y] = '-';
					y++;
					res[x][y] = '-';
				}
			}

			if (W & 1) {
				if (l & 1) {
					// go right
					for (int a = x+3; true; a += 3) {
						a = min(a, N-1);
						res[a][y] = '-';
						if (a == N-1) break;
					}
				} else {
					for (int a = y+3; true; a += 3) {
						a = min(a, N-1);
						res[x][a] = '-';
						if (a == N-1) break;
					}
				}
			}
			W >>= 1;
		}
		//for (string s : res) { cerr << s << '\n'; } cerr << '\n';
		return res;
	}
};

void test(int W) {
	cerr << "test " << W << '\n';
	auto v = StepHopJumpConstruct().construct(W);
	assert(comp(v) == W);
}

void my_tests() {
	for (int w = 0; w < 100; w++) {
		test(w);
	}
	for (int l = 1; l <= 29; l++) {
		test((1 << l)-1);
		test((1 << l));
		test((1 << l)+1);
	}
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::StepHopJumpConstruct;

// vim:ft=cpp
