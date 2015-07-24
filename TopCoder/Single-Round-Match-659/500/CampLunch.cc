#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class CampLunch {

public:

	static const int MOD = 1000000007;

	static const int MAXN = 17;
	static const int MAXM = 17;

	static const int MAXS = 1 << MAXM;

	int N, M;
	int A[MAXN][MAXM];

	int S;
	int dp[3][MAXS];

	int count(int n, int m, vector <string> a) {
		N = n;
		M = m;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < M; j++) {
				A[i][j] = a[i][j] - 'A';
			}
		}

		S = 1 << M;

		memset(dp, 0, sizeof(dp));

		dp[0][0] = 1;

		for(int r = 0; r < N; r++) {

			for(int i = 0; i < M; i++) {
				for(int s = 0; s < S; s++) {
					int v = dp[0][s];

					//cerr << r << ' ' << i << ' ' << s << ' ' << v << '\n';
					
					int p = 1 << A[r][i];

					if(s & p) {
						dp[1][s - p] += v;
						dp[1][s - p] %= MOD;
					} else {
						dp[1][s] += v;
						dp[1][s] %= MOD;

						if(i + 1 < M && !(s & (1 << A[r][i + 1]))) {
							dp[2][s] += v;
							dp[2][s] %= MOD;
						}

						dp[1][s + p] += v;
						dp[1][s + p] %= MOD;
						assert(s + p < S);
					}
				}

				for(int s = 0; s < S; s++) {
					dp[0][s] = dp[1][s];
					dp[1][s] = dp[2][s];
					dp[2][s] = 0;
				}
			}

		}

		return dp[0][0];

	}
};

// vim:ft=cpp
