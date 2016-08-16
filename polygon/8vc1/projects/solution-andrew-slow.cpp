#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MOD = 1e9 + 7;

const int MAXN = 203;
const int MAXK = 2000;
const int MAXA = 502;

int N, K;
int A[MAXN];
int dp[2][MAXN][MAXN * MAXA]; // time, number open, current value

int main() {
	scanf("%d %d", &N, &K);
	for(int i = 0; i < N; i++) scanf("%d", &A[i]);
	sort(A, A + N);
	reverse(A, A + N);
	dp[0][0][0] = 1;
	int tot = 0;
	for(int i = 0; i < N; i++) {
		int d = i & 1;
		int nd = !d;
		for(int k = i; k >= 0; k--) {
			for(int v = tot; v >= 0; v--) {
				dp[nd][k][v] = 0;
			}
		}
		for(int k = i; k >= 0; k--) {
			for(int v = tot; v >= 0; v--) {
				if(!dp[d][k][v]) continue;

				dp[nd][k][v] = (dp[nd][k][v] + ll(k + 1) * dp[d][k][v]) % MOD;

				dp[nd][k + 1][v + A[i]] += dp[d][k][v];
				if(dp[nd][k + 1][v + A[i]] >= MOD) dp[nd][k + 1][v + A[i]] -= MOD;

				if(k) {
					dp[nd][k - 1][v - A[i]] = (dp[nd][k - 1][v - A[i]] + ll(k) * dp[d][k][v]) % MOD;
				}
			}
		}
		tot += A[i];
	}
	int res = 0;
	for(int i = 0; i <= K; i++) {
		res += dp[N & 1][0][i];
		if(res >= MOD) res -= MOD;
	}
	printf("%d\n", res);
	return 0;
}
