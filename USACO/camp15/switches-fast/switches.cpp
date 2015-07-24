// https://www.overleaf.com/read/ncwrbgrjszqd

#include<bits/stdc++.h>
using namespace std;

const int MAXK = 17;
const int MAXS = 1 << MAXK;
const int MAXN = 2e5;

int K; // number of haybales
int S; // number of subsets (2^K)
int N; // number of cows

int sz[MAXS]; // precompute size of the subset
int choose[MAXK][MAXK]; // precompute binomial coefficients

// also stores mob
int cnt[MAXS][MAXK];

// also stores dp_v and Ans
int dp[MAXS];

int main() {
	cin >> K >> N;
	S = 1 << K;

	// precompute binomials
	for(int i = 0; i <= K; i++) {
		choose[i][0] = choose[i][i] = 1;
		for(int j = 1; j < i; j++) {
			choose[i][j] = choose[i - 1][j - 1] + choose[i - 1][j];
		}
	}

	// precompute subset sizes
	for(int m = 1; m < S; m++) {
		sz[m] = 1 + sz[m & (m - 1)];
	}

	// read in cows, store into cnt
	for(int i = 0; i < N; i++) {

		string s; cin >> s;

		// convert to bitmask
		int x = 0;
		for(char c : s) { x <<= 1, x |= (c - '0'); }

		int a, b; cin >> a >> b;

		for(int c = a; c <= b; c++) {
			cnt[x][c] ++;
		}
	}

	// convert cnt into mob
	for(int m = 0; m < S; m++) {
		for(int l = sz[m]; l >= 0; l--) {
			for(int k = 0; k < l; k++) {
				cnt[m][l] += choose[l][k] * cnt[m][k] * (((k ^ l) & 1) ? -1 : 1);
			}
		}
	}

	// compute dp
	for(int v = 0; v <= K; v++) {
		// compute dp_v, but only for |x| >= v
		// we don't need information about |x| < v
		// so we can reuse dp without touching already final values

		// initialize dp with mob
		for(int m = 0; m < S; m++) {
			if(v <= sz[m]) {
				dp[m] = cnt[m][v];
			} else {
				// don't touch dp
			}
		}

		// superset sum algorithm
		for(int i = 0; i < K; i++) {
			for(int x = 0; x < S; x++) {
				// ignore |x| < v
				if(sz[x] < v) continue;

				if(x & (1 << i)) continue;
				dp[x] += dp[x | (1 << i)];
			}
		}
	}

	// subset sum algorithm
	for(int i = 0; i < K; i++) {
		for(int x = S - 1; x > 0; x--) {
			if(x & (1 << i)) {
				dp[x] += dp[x ^ (1 << i)];
			}
		}
	}

	// dp now stores Ans
	int res = 0;
	for(int y = 0; y < S; y++) {
		res = max(res, dp[y]);
	}

	cout << res << '\n';
	return 0;
}
