#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

const int MAXV = 3010;
const int V = 3000;

ll dp[MAXV][MAXV];

class LShape {

	ll solve1(vector<int> x) {
		ll ans = 0;
		int N = int(x.size());
		sort(x.begin(), x.end());
		for (int i = 0; i < N; i++) {
			for (int j = i-1, k = i+1; j >= 0 || k < N;) {
				if (j < 0 || (k < N && x[k] - x[i] <= x[i] - x[j])) {
					ans += (j+1) * ll(x[k] - x[i]);
					++k;
				} else {
					ans += (N-k) * ll(x[i] - x[j]);
					--j;
				}
			}
		}
		return ans;
	}

	ll solve2(vector<int> x, vector<int> y) {
		int N = int(x.size());
		for (int i = 0; i <= V; i++) {
			for (int j = 0; j <= V; j++) {
				dp[i][j] = 0;
			}
		}
		for (int z = 0; z < N; z++) {
			dp[x[z]+1][y[z]+1] ++;
		}
		for (int i = 1; i <= V; i++) {
			for (int j = 1; j <= V; j++) {
				dp[i][j] += dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1];
			}
		}
		for (int i = 1; i <= V; i++) {
			for (int j = 1; j <= V; j++) {
				dp[i][j] += dp[i-1][j-1];
			}
		}

		ll ans = 0;
		for (int a = 0; a < N; a++) {
			for (int b = a+1; b < N; b++) {
				int xval = min(x[a], x[b]) - abs(x[a] - x[b]);
				int yval = min(y[a], y[b]) - abs(y[a] - y[b]);
				if (xval < 0 || yval < 0) {
					// += 0
				} else {
					ans += dp[xval][yval];
				}
			}
		}
		return ans;
	}

	vector<int> neg(vector<int> vec) {
		for (auto& v : vec) { v = V-1-v; }
		return vec;
	}

public:

	ll sumL(vector <int> y) {
		int N = int(y.size());
		vector<int> x(N);
		iota(x.begin(), x.end(), 0);
		return solve1(x) + solve1(y) + solve2(x,y) + solve2(neg(x), y) + solve2(x, neg(y)) + solve2(neg(x), neg(y));
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::LShape;

// vim:ft=cpp
