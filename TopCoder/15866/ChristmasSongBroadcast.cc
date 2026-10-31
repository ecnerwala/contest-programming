#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

struct dsu {
	vector<int> par;
	dsu(int N) : par(N, -1) {}
	int getPar(int a) { return par[a] == -1 ? a : (par[a] = getPar(par[a])); }
	bool merge(int a, int b) {
		a = getPar(a), b = getPar(b);
		if (a == b) return false;
		par[b] = a;
		return true;
	}
};

class ChristmasSongBroadcast {

public:

	int minimizeCost(int T, vector <int> A, vector <int> B, vector <string> directCost) {
		int N = int(A.size());
		vector<int> bestTower(N);

		for (int i = 0; i < N; i++) {
			const ll MOD = int(1e9)+7;
			ll a = A[i], b = B[i];
			ll v = 0;
			ll ans = b;
			while (true) {
				v += (MOD - b + a - 1) / a;
				if (v >= T) break;
				b += (MOD - b + a - 1) / a * a;
				b %= MOD;
				ans = min(ans, b);
			}
			bestTower[i] = int(ans);
		}

		vector<pair<ll, pair<int, int>>> edges;
		for (int i = 0; i < N; i++) {
			edges.push_back({bestTower[i], {i, N}});
			for (int j = i+1; j < N; j++) {
				edges.push_back({int(directCost[i][j]), {i, j}});
			}
		}

		sort(edges.begin(), edges.end());
		dsu d(N+1);
		ll ans = 0;
		for (auto it : edges) {
			if (d.merge(it.second.first, it.second.second)) ans += it.first;
		}
		return int(ans);
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::ChristmasSongBroadcast;

// vim:ft=cpp
