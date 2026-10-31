#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class AqaAsadiMinimizes {

public:

	double getMin(vector <int> P, int B0, int X, int Y, int N) {
		vector<int> A(N);
		for (int i = 0; i <= N-1; i++) {
			if (i < int(P.size())) {
				A[i] = P[i];
			} else if (i == int(P.size())) {
				A[i] = B0;
			} if (i > int(P.size())) {
				A[i] = (int64_t(A[i-1]) * X + Y) % 1000000007;
			}
		}
		vector<pair<int, int>> V(N);
		for (int i = 0; i < N; i++) V[i] = {A[i], i};
		sort(V.begin(), V.end());
		double ans = abs(A[0] - A[1]);
		for (int i = 0; i+1 < N; i++) {
			ans = min(ans, double(V[i+1].first - V[i].first) / abs(V[i+1].second - V[i].second));
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << AqaAsadiMinimizes().getMin({}, 234567890, 345678, 456789, 500000) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::AqaAsadiMinimizes;

// vim:ft=cpp
