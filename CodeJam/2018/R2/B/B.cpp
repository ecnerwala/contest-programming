#include<bits/stdc++.h>
using namespace std;

const int MAXR = 600;
const int MAXB = 600;

int dp[MAXR][MAXB];

void precomp() {
	memset(dp, 0, sizeof(dp));
	for (int a = 0; a <= 40; a++) {
		for (int b = 0; b <= 40; b++) {
			if (a == 0 && b == 0) continue;
			for (int i = MAXR - 1; i >= a; i--) {
				for (int j = MAXB - 1; j >= b; j--) {
					dp[i][j] = max(dp[i][j], dp[i-a][j-b] + 1);
				}
			}
		}
	}

	for (int i = 0; i < MAXR; i++) {
		for (int j = 0; j < MAXB; j++) {
			if (i) dp[i][j] = max(dp[i][j], dp[i-1][j]);
			if (j) dp[i][j] = max(dp[i][j], dp[i][j-1]);
		}
	}
}

int go(int r, int b) {
	return dp[r][b];
}

int main() {
	precomp();

	int T; scanf("%d", &T);

	for(int case_num = 1; case_num <= T; case_num ++) {
		int R, B; scanf("%d %d", &R, &B);

		int res = go(R, B);
		
		printf("Case #%d: %d\n", case_num, res);
	}

	return 0;
}
