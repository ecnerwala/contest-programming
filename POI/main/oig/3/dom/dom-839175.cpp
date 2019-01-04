#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 18;
const int MAXL = 1 << MAXN;
int N;
int L;
ll A[MAXL + 200];

const int MAXK = 50;
int K;
ll dp[MAXN][MAXK]; // max value taking at most k

void dfs(int level, int ind) {
	if (level == N-1) {
		dp[level][0] = 0;
		dp[level][1] = A[ind];
	} else {
		dfs(level + 1, 2 * ind);
		for (int i = 0; i <= K; i++) {
			dp[level][i] = dp[level+1][i];
		}
		dfs(level + 1, 2 * ind + 1);
		for (int i = K; i >= 1; i--) {
			ll v = 0;
			for (int j = 0; j < i; j++) {
				v = max(v, dp[level][j] + dp[level+1][i-1-j]);
			}
			dp[level][i] = v + A[ind];
		}
		dp[level][0] = 0;
	}
	for (int i = 1; i <= K; i++) {
		dp[level][i] = max(dp[level][i], dp[level][i-1]);
	}
}

int main() {
	scanf("%d %d", &N, &K);
	L = 1 << N;
	K /= 2;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < (1 << i); j++) {
			int a, b; scanf("%d %d", &a, &b);
			A[(1 << i) + j] = a+b;
		}
	}

	dfs(0, 1);
	printf("%lld\n", dp[0][K]);

	return 0;
}
