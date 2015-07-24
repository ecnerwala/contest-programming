#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;

const int MAXN = 3000;
ll dp[MAXN][MAXN];

ll c2[MAXN];
int main() {
	c2[0] = 0;
	for(int i = 0; i + 1 < MAXN; i++) {
		c2[i + 1] = c2[i] + i;
	}

	dp[0][0] = 1;
	for(int i = 1; i < MAXN; i++) {
		for(int c = 0; c <= i; c++) {
			// c is number of whole cows
			// h is number of half cows
			int h = (i - c) * 2;

			// two whole cows
			if(c >= 2) {
				dp[i][c] += dp[i - 1][c - 2] * c2[c] % MOD;
			}
			if(c >= 1 && h >= 1) {
				dp[i][c] += dp[i - 1][c - 1] * c % MOD * h % MOD;
			}
			if(h >= 2) {
				dp[i][c] += dp[i - 1][c] * c2[h] % MOD;
			}
			if(dp[i][c] >= MOD) dp[i][c] -= MOD;
			if(dp[i][c] >= MOD) dp[i][c] -= MOD;
		}
	}

	int T; cin >> T;
	for(int i = 0; i < T; i++) {
		int N; cin >> N;
		cout << dp[N][N] << '\n';
	}
}
