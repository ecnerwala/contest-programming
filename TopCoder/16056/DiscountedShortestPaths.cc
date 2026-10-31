#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int64_t dp[1<<19];
int64_t ndp[1<<19];

class DiscountedShortestPaths {

public:

	long long minimumCostSum(int N, vector <int> a, vector <int> b, vector <int> c, vector <int> d) {
		const int64_t INF = 1e18;
		int M = int(a.size());

		for (int m = 0; m < (1 << (N-1)); m++) {
			dp[m] = INF;
		}
		dp[0] = 0;

		vector<int> msks(M);
		for (int e = 0; e < M; e++) {
			if (a[e] != N-1) {
				msks[e] |= (1 << a[e]);
			}
			if (b[e] != N-1) {
				msks[e] |= (1 << b[e]);
			}
		}

		for (int z = 0; z < int(d.size()); z++) {
			int coupon = d[z];
			memcpy(ndp, dp, sizeof(dp[0]) * (1 << (N-1)));

			for (int e = 0; e < M; e++) {
				for (int m = 0; m < (1 << (N-1)); m++) {
					setmin(ndp[m ^ msks[e]], dp[m] + max(0, c[e] - coupon));
				}
			}

			memcpy(dp, ndp, sizeof(dp[0]) * (1 << (N-1)));
		}

		for (int e = 0; e < M; e++) {
			for (int m = 0; m < (1 << (N-1)); m++) {
				setmin(dp[m], dp[m ^ msks[e]] + c[e]);
			}
		}

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				ans += dp[(1 << i) ^ (j == N-1 ? 0 : (1 << j))];
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	int N = 20;
	vector<int> A;
	vector<int> B;
	vector<int> C;
	mt19937 mt(48);
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			A.push_back(i);
			B.push_back(j);
			C.push_back(mt() % int(1e9));
		}
	}
	vector<int> D(20);
	iota(D.begin(), D.end(), 10);
	ofstream fout("tbig.in");
	fout << N << '\n';
	for (int i = 0; i < int(A.size()); i++) {fout << ",{"[i==0] << A[i]; } fout << '}' << '\n';
	for (int i = 0; i < int(B.size()); i++) {fout << ",{"[i==0] << B[i]; } fout << '}' << '\n';
	for (int i = 0; i < int(C.size()); i++) {fout << ",{"[i==0] << C[i]; } fout << '}' << '\n';
	for (int i = 0; i < int(D.size()); i++) {fout << ",{"[i==0] << D[i]; } fout << '}' << '\n';

	cerr << DiscountedShortestPaths().minimumCostSum(N, A, B, C, D) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::DiscountedShortestPaths;

// vim:ft=cpp
