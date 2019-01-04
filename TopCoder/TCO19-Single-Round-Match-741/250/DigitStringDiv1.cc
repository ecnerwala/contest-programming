#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {
using ll = long long;

long long dp[20][2][2];
long long ndp[20][2][2];

class DigitStringDiv1 {

public:

	long long count(string S, int X) {
		int D = 0;
		for (int v = X; v; v /= 10) D++; // number of digits
		memset(dp, 0, sizeof(dp));
		dp[0][0][1] ++;
		for (int i = int(S.size()) - 1; i >= 0; i--) {
			int dig = (S[i] - '0');
			// don't take doens't change anything
			memcpy(ndp, dp, sizeof(dp));

			for (int d = 0, v = X; d <= D; d++, v /= 10) {
				if (d == D) assert(v % 10 == 0);
				if (dig == v % 10) {
					ndp[min(d+1, D)][0][dig==0] += dp[d][0][0] + dp[d][0][1];
					ndp[min(d+1, D)][1][dig==0] += dp[d][1][0] + dp[d][1][1];
				} else if (dig < v % 10) {
					ndp[min(d+1, D)][0][dig==0] += dp[d][0][0] + dp[d][0][1];
					ndp[min(d+1, D)][0][dig==0] += dp[d][1][0] + dp[d][1][1];
				} else {
					ndp[min(d+1, D)][1][dig==0] += dp[d][0][0] + dp[d][0][1];
					ndp[min(d+1, D)][1][dig==0] += dp[d][1][0] + dp[d][1][1];
				}
			}
			memcpy(dp, ndp, sizeof(dp));
		}
		return dp[D][1][0];
	}
};

}  // namespace ecnerwala

using ecnerwala::DigitStringDiv1;

// vim:ft=cpp
