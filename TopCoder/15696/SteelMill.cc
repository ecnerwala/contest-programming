#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;
const ll INF = 1e18;

class SteelMill {

public:

	long long cheapest(int goal, vector <int> shipmentCost, vector <int> shipmentSize, vector <int> costPerKg) {
		int N = int(shipmentCost.size());
		vector<pair<pair<ll, ll>, int>> options(N);
		for (int i = 0; i < N; i++) {
			options[i] = {{costPerKg[i], shipmentCost[i]}, shipmentSize[i]};
		}
		sort(options.begin(), options.end());

		vector<ll> dp(goal+1, INF);
		dp[0] = 0;
		for (auto it : options) {
			ll kiloCost = it.first.first;
			ll flatCost = it.first.second;
			int sz = it.second;
			for (int v = goal-1; v >= 0; v--) {
				int nv = min(v+sz, goal);
				dp[nv] = min(dp[nv], kiloCost * ll(nv - v) + flatCost + dp[v]);
			}
		}
		return dp[goal];
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << SteelMill().cheapest(int(1e6), vector<int>(100,int(1e9)), vector<int>(100, 1e6), vector<int>(100, int(1e9))) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::SteelMill;

// vim:ft=cpp
