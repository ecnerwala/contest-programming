#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {
const int V = 1010;

const int MOD = int(1e9) + 7;

class EraseToGCD {

public:

	int countWays(vector <int> S, int goal) {
		int res = 0;
		for (int i = goal; i <= V; i += goal) {
			int mob = 1;
			for (int v = i / goal, p = 2; v > 1; p++) {
				if (p * p > v) p = v;
				if (v % p != 0) continue;
				v /= p;
				mob *= -1;
				if (v % p == 0) {
					mob = 0; break;
				}
			}
			if (!mob) continue;
			int ways = 0;
			for (int a : S) {
				if (a % i == 0) {
					ways = (ways * 2 + 1) % MOD;
				}
			}
			res += ways * mob;
			res %= MOD;
		}
		res %= MOD;
		if (res < 0) res += MOD;
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::EraseToGCD;

// vim:ft=cpp
