#include <bits/stdc++.h>
using namespace std;

class SuccessiveSubtraction2 {

	static const int MAXN = 3e3;
	static const int MAXQ = 3e3;
	int N;
	int Q;

	int A[MAXN];

	int P[MAXN];

	int solve() {
		int minPref = 0;
		int maxSum = 0;
		int maxSumPref = 0;
		int maxSumSum = 0;

		int pref = 0;
		for(int i = 2; i < N; i++) {
			pref += A[i];
			minPref = min(minPref, pref);
			maxSum = max(maxSum, pref - minPref);
			maxSumPref = max(maxSumPref, maxSum - pref);
			maxSumSum = max(maxSumSum, maxSumPref + pref);
		}

		int res = 2 * maxSumSum;

		res += A[0];

		for(int i = 1; i < N; i++) {
			res -= A[i];
		}

		return res;
	}

public:

	vector <int> calc(vector <int> a, vector <int> p, vector <int> v) {
		N = a.size();
		for(int i = 0; i < N; i++) {
			A[i] = a[i];
		}
		Q = p.size();
		vector<int> res(Q);
		for(int i = 0; i < Q; i++) {
			A[p[i]] = v[i];
			res[i] = solve();
		}
		return res;
	}
};

// vim:ft=cpp
//
// a - ((b - c - d - e) - d) - e
// a - b + (c + d + e) - d + (e + f + g) - h - i
// a - (b - c) - (d - e)
// negate any 2 ranges, starting after a[1]
