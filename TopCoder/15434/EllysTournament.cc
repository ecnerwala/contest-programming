#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int MAXN = 600;
double lwin[MAXN][MAXN];
double rwin[MAXN][MAXN];
double dp[MAXN][MAXN];

class EllysTournament {

public:

	double getChance(int N, int K, vector <int> ratings) {
		K--;
		memset(lwin, 0, sizeof(lwin));
		memset(rwin, 0, sizeof(rwin));

		for (int i = 0; i < N; i++) {
			lwin[i][i] = rwin[i][i] = 1;
		}

		for (int d = 1; d < N; d++) {
			for (int i = 0, j = d; j < N; i++, j++) {
				dp[i][j] = 0;
				for (int k = i; k < j; k++) {
					dp[i][j] += lwin[i][k] * rwin[k+1][j];
				}
				dp[i][j] /= ratings[i] + ratings[j];

				lwin[i][j] = 0;
				for (int k = i+1; k <= j; k++) {
					lwin[i][j] += ratings[i] * dp[i][k] * lwin[k][j] / (j - i);
				}
				rwin[i][j] = 0;
				for (int k = i; k < j; k++) {
					rwin[i][j] += ratings[j] * rwin[i][k] * dp[k][j] / (j - i);
				}
			}
		}

		return rwin[0][K] * lwin[K][N-1];
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::EllysTournament;

// vim:ft=cpp
