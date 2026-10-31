#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class SwapTheString {

public:

	long long findNumberOfSwaps(string P, int A0, int X, int Y, int N, int K) {
		vector<long long> A(N);
		A[0] = A0;
		for (int i = 1; i <= N-1; i++) {
			A[i] = (A[i-1] * X + Y) % 1812447359;
		}
		string S = P; S.resize(N);
		for (int i = int(P.size()); i <= N-1; i++) {
			S[i] = (char)(A[i] % 26 + 'a');
		}
		long long ans = 0;
		for (int k = 0; k < K; k++) {
			vector<int> cnt(26, 0);
			for (int i = k; i < N; i += K) {
				int v = S[i] - 'a';
				for (int u = 0; u < v; u++) ans += cnt[u];
				cnt[v]++;
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << SwapTheString().findNumberOfSwaps("", 102340, 12345678, 12345, 200000, 1) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::SwapTheString;

// vim:ft=cpp
