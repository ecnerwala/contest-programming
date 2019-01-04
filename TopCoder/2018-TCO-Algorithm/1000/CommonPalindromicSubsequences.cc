#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

const int MAXN = 70;
int N[2];
int S[2][MAXN];
int nxt[2][MAXN][30]; // nxt inclusive
int prv[2][MAXN][30]; // prv inclusive
ll dp[MAXN][MAXN][MAXN][MAXN];

// inclusive range
// number of nonempty things in this range
ll rec(int l0, int r0, int l1, int r1) {
	assert(l0 <= r0 && l1 <= r1);
	if (dp[l0][r0][l1][r1]) {
		return dp[l0][r0][l1][r1];
	}
	ll res = 0;
	for (int c = 0; c < 26; c++) {
		int nl0 = nxt[0][l0][c];
		int nl1 = nxt[1][l1][c];

		int nr0 = prv[0][r0][c];
		int nr1 = prv[1][r1][c];
		if (nl0 <= nr0 && nl1 <= nr1) {
			// there's something in this range
			res ++; // +1 for odd
			if (nl0 < nr0 && nl1 < nr1) {
				res ++; // +1 for even
				if (nl0 + 1 <= nr0 - 1 && nl1 + 1 <= nr1 - 1) {
					res += rec(nl0 + 1, nr0 - 1, nl1 + 1, nr1 - 1);
				}
			}
		}
	}
	return dp[l0][r0][l1][r1] = res;
}

ll go() {
	memset(dp, 0, sizeof(dp));
	for (int z = 0; z < 2; z++) {
		S[z][0] = S[z][N[z]+1] = -1;
		for (int c = 0; c < 26; c++) {
			int p = 0;
			for (int i = 1; i <= N[z]; i++) {
				if (S[z][i] == c) {
					p = i;
				}
				prv[z][i][c] = p;
			}
			int n = N[z]+1;
			for (int i = N[z]; i >= 1; i--) {
				if (S[z][i] == c) {
					n = i;
				}
				nxt[z][i][c] = n;
			}
		}
	}
	return rec(1, N[0], 1, N[1]);
}

class CommonPalindromicSubsequences {

public:

	ll count(string A, string B) {
		N[0] = int(A.size());
		for (int i = 1; i <= N[0]; i++) {
			assert('a' <= A[i-1] && A[i-1] <= 'z');
			S[0][i] = A[i-1] - 'a';
			assert(0 <= S[0][i] && S[0][i] < 26);
		}
		N[1] = int(B.size());
		for (int i = 1; i <= N[1]; i++) {
			assert('a' <= B[i-1] && B[i-1] <= 'z');
			S[1][i] = B[i-1] - 'a';
			assert(0 <= S[1][i] && S[1][i] < 26);
		}
		ll res = go();
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::CommonPalindromicSubsequences;

// vim:ft=cpp
