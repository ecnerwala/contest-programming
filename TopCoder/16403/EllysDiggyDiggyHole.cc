#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class EllysDiggyDiggyHole {

	long long solve_1D(int N, int M, int X1, int A, int B) {
		vector<int> X; X.reserve(N);
		for (int i = 0; i < N; i++) {
			X.push_back(X1);
			X1 = int((int64_t(X1) * A + B) & (M-1));
		}
		auto md = X.begin() + N/2;
		nth_element(X.begin(), md, X.end());
		int mx = *md;
		long long ans = 0;
		for (int x : X) ans += abs(x - mx);
		return ans;
	}
public:

	long long getMin2(int N, int M, int X1, int Y1, int A, int B, int C, int D) {
		return solve_1D(N, M, X1, A, B) + solve_1D(N, M, Y1, C, D);
	}

	long long getMin(int N, int M, int X1, int Y1, int A, int B, int C, int D) {
		vector<int> X; X.reserve(N);
		vector<int> Y; Y.reserve(N);
		for (int i = 0; i < N; i++) {
			X.push_back(X1);
			Y.push_back(Y1);
			X1 = int((int64_t(X1) * A + B) & (M-1));
			Y1 = int((int64_t(Y1) * C + D) & (M-1));
		}
		nth_element(X.begin(), X.begin() + N/2, X.end());
		nth_element(Y.begin(), Y.begin() + N/2, Y.end());
		int mx = X[N/2];
		int my = Y[N/2];
		long long ans = 0;
		for (int i = 0; i < N; i++) ans += abs(X[i] - mx) + abs(Y[i] - my);
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::EllysDiggyDiggyHole;

// vim:ft=cpp
