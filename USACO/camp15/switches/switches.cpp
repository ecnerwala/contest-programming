#include<bits/stdc++.h>
using namespace std;

const int MAXK = 17;
const int MAXS = 1 << MAXK;
const int MAXN = 2e5;

int K, S;

int N;

// precompuete number of 1's
int len[MAXS];

// counts of cows who like subset of s w/ size == k
int cnt[MAXS][MAXK];

// convolve with 1 to get number of happy cows
int dp[MAXS];

int choose[MAXK][MAXK];


int main() {
	cin >> K >> N;
	S = 1 << K;

	for(int i = 0; i <= K; i++) {
		choose[i][0] = choose[i][i] = 1;
		for(int j = 1; j < i; j++) {
			choose[i][j] = choose[i - 1][j - 1] + choose[i - 1][j];
		}
	}

	for(int m = 1; m < S; m++) {
		len[m] = 1 + len[m & (m - 1)];
	}

	for(int i = 0; i < N; i++) {
		string s; cin >> s;
		int m = 0;
		for(char c : s) { m <<= 1, m |= (c - '0'); }
		int a, b; cin >> a >> b;
		for(int c = a; c <= b; c++) {
			cnt[m][c] ++;
		}
	}


	for(int m = 0; m < S; m++) {
		for(int l = len[m]; l >= 0; l--) {
			for(int k = 0; k < l; k++) {
				cnt[m][l] += choose[l][k] * cnt[m][k] * (((k ^ l) & 1) ? -1 : 1);
			}
		}
	}

	/*
	for(int m = 0; m < S; m++) {
		for(int v = m; true; v = (v - 1) & m) {
			dp[v] += cnt[m][len[v]];
			if(v == 0) break;
		}
	}
	*/

	for(int l = 0; l <= K; l++) {
		for(int m = 0; m < S; m++) {
			if(len[m] >= l) {
				dp[m] = cnt[m][l];
			} else {
				assert(cnt[m][l] == 0);
			}
		}

		for(int i = 0; i < K; i++) {
			for(int m = 0; m < S; m++) {
				if(len[m] < l) continue;
				if(m & (1 << i)) continue;
				dp[m] += dp[m | (1 << i)];
			}
		}
	}

	int res = 0;
	for(int m = 0; m < S; m++) {
		int val = 0;
		for(int v = m; true; v = (v - 1) & m) {
			val += dp[v];
			if(v == 0) break;
		}
		res = max(res, val);
	}

	cout << res << '\n';
	return 0;
}
