#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class Diophantine {

public:

	long long countSolutions(int N, int D) {
		vector<bool> isPrime(50000, true);
		isPrime[0] = isPrime[1] = false;
		vector<int> P;
		for (int p = 2; int(P.size()) < N+D; p++) {
			if (!isPrime[p]) continue;
			P.push_back(p);

			for (int j = p + p; j < int(isPrime.size()); j += p) {
				isPrime[j] = false;
			}
		}

		vector<ll> Q;
		for (int i = 0; i < N; i++) {
			Q.push_back(P[i] * P[i + D]);
		}
		//for (ll v : Q) { cerr << v << ' '; } cerr << '\n';

		ll ans = 0;
		unordered_map<ll, int> mp;
		for (int y = 0; y < N; y++) {
			for (int x = 0; x <= y && Q[0] + Q[x] + Q[y] + Q[y] <= Q.back(); x++) {
				mp[Q[0] + Q[x] + Q[y]] ++;
			}

			for (int z = N-1; z >= y && Q[z] > Q[y]; z--) {
				ans += mp[Q[z] - Q[y]];
			}
			//cerr << y << '\n';
		}
		return ans;

	}
};

void my_tests() {
	// FILL IN TESTS
	Diophantine().countSolutions(2500, 2500);
}

}  // namespace ecnerwala

using ecnerwala::Diophantine;

// vim:ft=cpp
