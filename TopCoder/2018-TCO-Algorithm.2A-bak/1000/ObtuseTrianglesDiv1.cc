#include <bits/stdc++.h>
using namespace std;

class ObtuseTrianglesDiv1 {

	typedef long long ll;

	ll make_tri(int x, int y, int z) {
		return ll(x) * ll(1e8) + ll(y) * ll(1e4) + ll(z);
	}

public:

	vector<ll> findObtuseTriangles(int a, int n) {
		if (a == 1) return vector<ll>{};
	}
};

// vim:ft=cpp
