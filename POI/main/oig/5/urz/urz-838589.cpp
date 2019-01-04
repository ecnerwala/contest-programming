#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;
int P[MAXN];

int dp[MAXN][2];

int main() {
	scanf("%d", &N);
	P[0] = -1;
	for (int i = 1; i < N; i++) {
		scanf("%d", &P[i]);
		P[i] --;
	}
	for (int i = N-1; i >= 0; i--) {
		dp[i][0] += dp[i][1];
		dp[i][1] ++;
		dp[i][1] = max(dp[i][1], dp[i][0]);

		int p = P[i];
		if (p == -1) continue;
		dp[p][1] += dp[i][0];
		dp[p][0] = max(dp[p][0], dp[i][1] - dp[i][0]);
	}

	printf("%d\n", dp[0][1]);

	return 0;
}
