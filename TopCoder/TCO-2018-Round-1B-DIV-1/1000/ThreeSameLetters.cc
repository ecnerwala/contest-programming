#include <bits/stdc++.h>
using namespace std;

class ThreeSameLetters {

	typedef long long ll;
	const ll MOD = ll(1e9) + 7;

	static const int MAXN = 30;
	ll dp[MAXN][2];

public:

	int countStrings(int L, int S) {
		if (L < 3) return 0;
		dp[0][0] = 0;
		dp[0][1] = 1;
		for (int i = 0; i < L; i++) {
			dp[i+1][0] = (dp[i][0] + dp[i][1]) % MOD * (S-1) % MOD;
			dp[i+1][1] = dp[i][0];
		}
		ll res = 0;
		for (int i = 0; i <= L - 3; i++) {
			int j = L - 3 - i;
			res += (dp[i][0] + dp[i][1]) % MOD * ((dp[j][0] + dp[j][1]) % MOD) % MOD;
			res %= MOD;
		}
		res *= S;
		res %= MOD;
		return int(res);
	}
};

// vim:ft=cpp
