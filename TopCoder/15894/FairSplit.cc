#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class FairSplit {

public:

	bool checkGood(ll a, ll b) {
		ll sum = (b-a+1) * (a+b) / 2;
		if (sum % 2 != 0) return false;

		if ((b-a+1) % 2 == 0) {
			return true;
		} else {
			ll l = (b-a+1)/2;
			assert(b == a + l + l);
			ll hiSum = (a+l+1 + a+l+l) * l / 2;
			ll loSum = (a+a+l) * (l+1) / 2;
			assert(hiSum + loSum == sum);
			if (hiSum >= loSum) {
				return true;
			}
			return false;
		}
	}

	ll countPrefSlow(ll Amax, ll Bmax) {
		if (Amax == 0 || Bmax == 0) return 0;
		Amax = min(Amax, Bmax); // A <= B

		ll slowAns = 0;
		for (ll a = 1; a <= Amax; a++) {
			for (ll b = a; b <= Bmax; b++) {
				slowAns += checkGood(a,b);
			}
		}

		return slowAns;
	}

	ll countPref(ll Amax, ll Bmax) {
		if (Amax == 0 || Bmax == 0) return 0;
		Amax = min(Amax, Bmax); // A <= B

		ll ans = 0;

		Bmax++; // let's make A < B

		// B-A (# items) is a multiple of 4: we can always do it
		// Pick the mod
		for (int m = 0; m < 4; m++) {
			ll Am4 = (Amax + m) / 4;
			ll Bm4 = (Bmax + m) / 4;
			assert(Am4 <= Bm4);
			ans += Bm4 * (Bm4 - 1) / 2;
			ans -= (Bm4 - Am4) * (Bm4 - Am4 - 1) / 2;
		}

		// A+B-1 is a multiple of 4
		// len is 2*l+1
		// Only works if A <= l^2
		// Complementary count
		for (int ma = 0, mb = 3; ma < 4; ma++, mb--) {
			ll Am4 = (Amax + ma) / 4;
			ll Bm4 = (Bmax + mb) / 4;
			assert(ma != mb);
			if (ma > mb) {
				// B is automatically greater
				Bm4++;
			}
			Am4 = min(Am4, Bm4 - 1);
			if (Am4 >= 1) {
				ans += Bm4 * (Bm4 - 1) / 2;
				ans -= (Bm4 - Am4) * (Bm4 - Am4 - 1) / 2;
			}
		}

		for (ll l = 0; l * l < Amax; l++) {
			// Number such that A > l^2
			ll vMax = min(Bmax - (2 * l + 1), Amax);
			ll vMin = l*l;
			if (vMin < vMax) {
				ans -= (vMax + (l%2)) / 2 - (vMin + (l%2)) / 2;
			}
		}

		return ans;
	}

	long long count(int loA, int hiA, int loB, int hiB) {
		return countPref(hiA, hiB) - countPref(loA-1, hiB) - countPref(hiA, loB-1) + countPref(loA-1, loB-1);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::FairSplit;

// vim:ft=cpp
