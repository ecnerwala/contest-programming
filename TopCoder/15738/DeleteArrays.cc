#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class DeleteArrays {

public:

	vector<long long> doDelete(int NA, int NB, int NC, long long wx, long long wy, long long wz) {
		vector<ll> A(NA);
		vector<ll> B(NB);
		vector<ll> C(NC);
		A[0] = 33;
		A[1] = 42;
		for (int i = 2; i <= NA-1; i++) {
			A[i] = (5*A[i-1] + 7*A[i-2]) % 1000000007 + 1;
		}

		B[0] = 13;
		for (int i = 1; i <= NB-1; i++) {
			B[i] = (11*B[i-1]) % 1000000007 + 1;
		}

		C[0] = 7;
		C[1] = 2;
		for (int i = 2; i <= NC-1; i++) {
			C[i] = (5*C[i-1] + 7*C[i-2]) % 1000000007 + 1;
		}

		sort(A.begin(), A.end());
		sort(B.begin(), B.end());
		sort(C.begin(), C.end());

		vector<ll> V[3];
		V[0] = A;
		V[1] = B;
		V[2] = C;
		int N[3];
		N[0] = NA;
		N[1] = NB;
		N[2] = NC;
		ll W[3][3];
		memset(W, 0, sizeof(W));
		W[0][1] = W[1][0] = wx;
		W[1][2] = W[2][1] = wy;
		W[0][2] = W[2][0] = wz;

		ll S = 0;
		for (int z = 0; z < 3; z++) {
			for (int i = 0; i < N[z]; i++) {
				S += V[z][i];
			}
		}

		for (int z = 0; z < 3; z++) {
			int x = (z+1)%3, y = (z+2) % 3;
			if (N[z] >= N[x] + N[y]) {
				ll ans = 0;
				for (int i = 0; i < N[z] - N[x] - N[y]; i++) {
					ans += V[z][i];
				}
				return {ans, N[x] * W[z][x] + N[y] * W[z][y] + (S - ans)};
			}
		}
		int s = (N[0] + N[1] + N[2]) / 2;
		if ((N[0] + N[1] + N[2]) & 1) {
			vector<ll> ans;
			for (int z = 0; z < 3; z++) {
				int x = (z+1)%3, y = (z+2) % 3;
				vector<ll> cnd({V[z][0], W[x][y] * (s - (N[z]-1)) + W[y][z] * (s - N[x]) + W[x][z] * (s - N[y]) + (S - V[z][0])});
				if (z == 0) {
					ans = cnd;
				} else {
					ans = min(ans, cnd);
				}
			}
			return ans;
		} else {
			return {0, W[0][1] * (s - N[2]) + W[1][2] * (s - N[0]) + W[2][0] * (s - N[1]) + S};
		}
	}
};

void my_tests() {
	// FILL IN TESTS
	DeleteArrays().doDelete(int(1e5),int(1e5),int(1e5), 0,0,0);
	
}

}  // namespace ecnerwala

using ecnerwala::DeleteArrays;

// vim:ft=cpp
