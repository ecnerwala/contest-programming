#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class RandomSelection {

	using ll = long long;
	using ld = long double;
	const ld INF = 1e99;

public:

	double go(vector<ll> A) {
		int N = int(A.size());
		sort(A.begin(), A.end());
		ld denom = 0;
		ld ans = -INF;
		for (int i = N-1; i >= 0; i--) {
			denom -= log(ld(A[i]));

			ll cur = A[i];
			ll prv = i ? A[i-1] : 0;

			if (cur == prv) continue;

			int nBig = N-i;
			ld diffPow = log(ld(cur)) * (nBig + 1) + log(1 - pow(ld(prv) / ld(cur), nBig+1));
			ld ev = denom + log(ld(nBig)) - log(ld(nBig+1)) + diffPow;
			if (ev >= ans) swap(ans, ev);
			//assert(ans >= ev);
			ans += log(1 + exp(ev - ans));
		}
		cerr << ans << '\n';
		return exp(ans);
	}

	double expectedMaximum(int N, int T, int seed, int Amod, vector <int> Aprefix) {
		vector<ll> A(N);
		for (int i = 0; i <= int(Aprefix.size())-1; i++) {
			A[i] = Aprefix[i];
		}
		ll state = seed;
		for (int i = int(Aprefix.size()); i <= N-1; i++) {
			state = (1103515245 * state + 12345) % (1ll << 31);
			A[i] = T + (state % Amod);
		}

		return go(std::move(A));
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int i = 0; i < 100; i++) {
		cerr << int(1e9) + i+1 << ",";
	}
	cerr << RandomSelection().expectedMaximum(37966, 787981100, 4915907, 4724, {2500, 2169, 3557, 4652, 2882, 159, 2445, 2616, 3329, 3835, 837, 4584, 3665, 2168, 1058, 2538, 3959, 893, 4218, 2531}) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::RandomSelection;

// vim:ft=cpp
