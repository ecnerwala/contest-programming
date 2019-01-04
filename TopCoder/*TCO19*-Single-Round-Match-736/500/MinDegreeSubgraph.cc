#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class MinDegreeSubgraph {

public:

	bool hasDense(ll n, ll m, ll k) {
		assert(0 <= m && m <= n * (n-1) / 2);
		if (n >= k+1 && m >= k * (k+1) / 2) return true;
		return false;
	}
	bool hasNotDense(ll n, ll m, ll k) {
		// in fact, none are dense
		if (n <= k) return true;
		if (k == 0) {
			return false;
		}
		return m <= (n - k) * (k-1) + k * (k-1) / 2;
	}

	string exists(int nn, ll m, int k) {
		bool y = hasDense(nn, m, k);
		bool n = hasNotDense(nn, m, k);
		assert(y || n);
		if (y && n) return "SOME";
		else if (y) return "ALL";
		else if (n) return "NONE";
		else assert(false);
		return "";
	}
};

}  // namespace ecnerwala

using ecnerwala::MinDegreeSubgraph;

// vim:ft=cpp
