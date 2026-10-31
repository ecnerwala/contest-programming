#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class NRooksPosition {

public:

	long long minSteps(int N, vector <int> Xprefix, vector <int> Yprefix, int A, int B, int C) {
		vector<int> X(N);
		vector<int> Y(N);
		int L = int(Xprefix.size());
		for (int i = 0; i < L; i++) {
			X[i] = Xprefix[i];
			Y[i] = Yprefix[i];
		}
		for (int i = L; i < N; i++) {
			X[i] = int((int64_t(A) * X[i-1] + int64_t(B) * X[i-2]) % C);
			Y[i] = int((int64_t(A) * Y[i-1] + int64_t(B) * Y[i-2]) % C);
		}
		sort(X.begin(), X.end());
		sort(Y.begin(), Y.end());
		for (int i = 0; i < N; i++) {
			X[i] -= i;
			Y[i] -= i;
		}
		sort(X.begin(), X.end());
		sort(Y.begin(), Y.end());
		int64_t ans = 0;
		for (int i = 0; i < N; i++) ans += abs(X[i] - X[N/2]) + abs(Y[i] - Y[N/2]);
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::NRooksPosition;

// vim:ft=cpp
