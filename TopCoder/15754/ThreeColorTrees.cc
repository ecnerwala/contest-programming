#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

bool check(vector<int> par) {
	par.insert(par.begin(), -1);
	int N = int(par.size());
	vector<vector<double>> dp(N, vector<double>({1., 0., 2.}));
	for (int i = N-1; i > 0; i--) {
		assert(par[i] < i);
		vector<double> nways(3);
		for (int x = 0; x <= 2; x++) {
			for (int y = 0; y <= 2; y++) {
				if (1 + x + y < 3) continue;
				nways[min(x, y+1)] += dp[par[i]][x] * dp[i][y];
			}
		}
		dp[par[i]] = nways;
	}
	double v = dp[0][0] + dp[0][1] + dp[0][2];
	cerr << "N=" << N << " " << "v=" << v << " " << "pow(2.62,N)=" << pow(2.62, N) << '\n';
	return v >= pow(2.62, N);
}

class ThreeColorTrees {

public:

	vector <int> construct(int N) {
		vector<int> res(N-1);
		iota(res.begin(), res.end(), 0);
		mt19937 mt(48);
		/*
		for (int i = 1; i < N; i++) {
			int p3 = 1;
			while (i % (p3 * 3) == 0) p3 *= 3;
			res[i-1] = i - p3;
		}
		*/
		for (int i = 1; i < N; i++) {
			res[i-1] = (i+1)/2;
		}
		for (int i = (N / 3); i < N; i++) {
			res[i-1] = i - N/3;
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
	assert(check({0}));
	assert(check({0, 1, 0, 3, 4, 0, 0, 7, 8 }));
	for (int i = 1; i <= 100; i++) {
		vector<int> v = ThreeColorTrees().construct(i);
		assert(int(v.size()) == i-1);
		assert(check(v));
	}
}

}  // namespace ecnerwala

using ecnerwala::ThreeColorTrees;

// vim:ft=cpp
