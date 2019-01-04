#include <bits/stdc++.h>
using namespace std;

class RndSubTree {

	typedef long long ll;
	static const ll MOD = ll(1e9) + 7;
	static const ll INV2 = (MOD + 1) / 2;

	ll powmod(ll a, ll b, ll m=MOD) {
		assert(b >= 0);
		a %= m;

		ll r = 1;
		while (b) {
			if (b & 1) {
				r *= a, r %= m;
			}
			a *= a, a %= m;
			b /= 2;
		}
		return r % m;
	}

	ll inv(ll a, ll m=MOD) {
		return m - inv(m % a, a) * m / a;
	}

	static const int MAXK = 2020;

	pair<ll, ll> dp[MAXK];
	ll choose[MAXK];

	ll go(int K) {
		dp[0] = {0,0};
		choose[0] = 1;
		for (int i = 1; i <= K; i++) {
			dp[i] = {0, 0};
			for (int l = 0; l <= i-1; l++) {
				int r = i-1-l;
				ll p = choose[l];
				dp[i].first += (dp[l].first + dp[r].first + (l + dp[l].second) * (r+1) + (r + dp[r].second) * (l+1)) % MOD * p % MOD;
				dp[i].first %= MOD;
				dp[i].second += (l + dp[l].second + r + dp[r].second) % MOD * p % MOD;
				dp[i].second %= MOD;
			}
			choose[i] = 0;
			for (int j = i; j > 0; j--) {
				choose[j] += choose[j-1];
				choose[j] %= MOD;
			}
			for (int j = 0; j <= i; j++) {
				choose[j] *= INV2;
				choose[j] %= MOD;
			}
		}
		return dp[K].first;
	}

public:

	int count(int k) {
		return (go(k) % MOD + MOD) % MOD;
	}
};

// vim:ft=cpp
