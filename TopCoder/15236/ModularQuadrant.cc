#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long; 

class ModularQuadrant {

public:

	// sum of [0,r] and [0,c]
	ll sum(ll r, ll c) {
		if (r < 0 || c < 0) return 0;
		if (r < c) swap(r, c);
		assert(r >= c);

		ll res = 0;

		// 0 * 1 + 1 * 3 + 2 * 5 + ... + c%3 * (2c+1)
		for (int z = 0; z < 3; z++) {
			ll lo = z;
			ll hi = (c+3-z) / 3 * 3 + z - 3;
			ll cnt = (hi - lo) / 3 + 1;
			res += z * (((2*lo+1) + (2*hi+1)) * cnt / 2);
		}

		for (int z = 0; z < 3; z++) {
			ll cnt = (r+3-z) / 3 - (c+3-z) / 3;
			res += z * cnt * (c+1);
		}
		return res;
	}

	ll sum(int r1, int r2, int c1, int c2) {
		r1--, c1--;
		return sum(r2, c2) + sum(r1, c1) - sum(r2, c1) - sum(r1, c2);
	}
};

}  // namespace ecnerwala

using ecnerwala::ModularQuadrant;

// vim:ft=cpp
