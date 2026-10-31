#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class Tigerrymander {

public:

	long long minTigers(long long N, int TV, int OV) {
		std::vector<std::pair<ll, int>> pfac;
		std::vector<ll> factors;
		factors.push_back(1);
		for (ll p = 2, v = N; v > 1; p++) {
			if (p * p > v) {
				p = v;
			}
			if (v % p == 0) {
				int e = 0;
				while (v % p == 0) {
					v /= p;
					e++;
				}
				pfac.push_back({p, e});
				int s = int(factors.size());
				factors.reserve(s * (e+1));
				for (int i = 0; i < e * s; i++) {
					factors.push_back(factors[i] * p);
				}
			}
		}
		std::vector<ll> dp = factors; // min number of tigers to win with something of this size
		for (int i = 0; i < int(factors.size()); i++) {
			for (int j = 0; j < i; j++) {
				if (factors[i] % factors[j] != 0) continue;
				ll c = factors[i] / factors[j];
				ll x = (OV * c) / (TV + OV) + 1;
				dp[i] = std::min(dp[i], dp[j] * x);
			}
		}
		return dp.back();
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::Tigerrymander;

// vim:ft=cpp
