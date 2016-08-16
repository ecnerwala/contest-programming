#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class BracketSequenceDiv1 {

public:

	static const int MAXN = 60;
	int N;
	long long dp[MAXN][MAXN]; // number of ways starting exactly with i
	long long count(string s) {
		N = int(s.size());
		memset(dp, 0, sizeof(dp));

		for(int l = 1; l <= N; l++) {
			for(int i = 0; i + l <= N; i++) {
				int j = i + l - 1; // inclusive
				for(int k = i + 1; k <= j; k++) {
					if((s[i] == '(' && s[k] == ')') || (s[i] == '[' && s[k] == ']')) {
						long long v = 1;
						for(int m = i + 1; m <= k - 1; m++) {
							v += dp[m][k - 1];
						}
						long long w = 1;
						for(int m = k + 1; m <= j; m++) {
							w += dp[m][j];
						}
						dp[i][j] += v * w;
					}
				}
			}
		}
		long long res = 0;
		for(int i = 0; i < N; i++) {
			res += dp[i][N - 1];
		}
		return res;
	}
};

// vim:ft=cpp
