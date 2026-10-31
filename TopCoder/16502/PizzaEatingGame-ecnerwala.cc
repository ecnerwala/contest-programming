#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class PizzaEatingGame {

public:

	int eat(vector <int> olives) {
		const int INF = 1e9;
		int N = int(olives.size());
		vector<vector<int>> dp(N, vector<int>(N+1));
		for (int i = 0; i < N; i++) {
			dp[i][0] = 0;
		}
		for (int l = 1; l < N; l++) {
			for (int i = 0; i < N; i++) {
				dp[i][l] = -INF;
				for (int m = 0; m < l; m++) {
					dp[i][l] = max(dp[i][l], olives[(i+m)%N] - dp[i][m] - dp[(i+m+1)%N][l-1-m]);
				}
			}
		}
		int ans = -INF;
		for (int i = 0; i < N; i++) {
			ans = max(ans, olives[i] - dp[(i+1)%N][N-1]);
		}
		int tot = std::accumulate(olives.begin(), olives.end(), 0);
		return (tot + ans) / 2;
	}
};

void my_tests() {
	mt19937 mt(48);
	// FILL IN TESTS
	vector<int> v(500);
	for (int& i : v) i = mt() % 1001;
	cerr << PizzaEatingGame().eat(v) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::PizzaEatingGame;

// vim:ft=cpp
