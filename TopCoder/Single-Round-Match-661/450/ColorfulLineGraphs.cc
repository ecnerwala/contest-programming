#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class ColorfulLineGraphs {

public:

	ll exp(ll a, ll b, ll m) {
		ll res = 1;
		while(b) {
			if(b & 1) res *= a, res %= m;
			a *= a, a %= m;
			b /= 2;
		}
		return res;
	}

	int countWays(long long N, long long K, int M) {

		ll res = 1;

		for(ll i = 1; i <= M; i++) {
			res *= (i * (K - 1) + 1) % M;
			res %= M;
		}

		res = exp(res, N / M, M);

		for(ll i = 1; i <= N % M; i++) {
			res *= (i * (K - 1) + 1) % M;
			res %= M;
		}

		return int(res % M);
	}
};

// vim:ft=cpp
