#include<bits/stdc++.h>
using namespace std;

const int MOD = 1e6;

int N[4];
int dp[2][40][40][40][4][4];

inline bool bad(int a, int b, int c) {
	return ((a ^ b) | (a ^ c)) ^ 3;
}

int main() {
	scanf("%d %d %d %d", &N[0], &N[1], &N[2], &N[3]);
	if(N[0] + N[1] + N[2] + N[3] <= 1) {
		printf("1\n");
		return 0;
	}

	dp[0][0][0][0][0][0] = 1;
	dp[0][1][0][0][0][1] = 1;
	dp[0][0][1][0][0][2] = 1;
	dp[0][0][0][1][0][3] = 1;

	dp[0][1][0][0][1][0] = 1;
	dp[0][2][0][0][1][1] = 1;
	dp[0][1][1][0][1][2] = 1;
	dp[0][1][0][1][1][3] = 1;

	dp[0][0][1][0][2][0] = 1;
	dp[0][1][1][0][2][1] = 1;
	dp[0][0][2][0][2][2] = 1;
	dp[0][0][1][1][2][3] = 1;

	dp[0][0][0][1][3][0] = 1;
	dp[0][1][0][1][3][1] = 1;
	dp[0][0][1][1][3][2] = 1;
	dp[0][0][0][2][3][3] = 1;

	for(int s = 2; s < N[0] + N[1] + N[2] + N[3]; s++) {
		memset(dp[!(s & 1)], 0, sizeof(dp[0]));
		for(int a = 0; a <= N[0]; a++) {
			for(int b = 0; b <= N[1]; b++) {
				for(int c = 0; c <= N[2]; c++) {
					int d = s - a - b - c;
					if(d < 0 || d > N[3]) continue;
					for(int x = 0; x < 4; x++) {
						for(int y = 0; y < 4; y++) {
							int v = dp[s & 1][b][c][d][x][y] % MOD;
							if(v) {
								if(!bad(x, y, 0)) {
									dp[!(s & 1)][b][c][d][y][0] += v;
								}
								if(!bad(x, y, 1)) {
									dp[!(s & 1)][b + 1][c][d][y][1] += v;
								}
								if(!bad(x, y, 2)) {
									dp[!(s & 1)][b][c + 1][d][y][2] += v;
								}
								if(!bad(x, y, 3)) {
									dp[!(s & 1)][b][c][d + 1][y][3] += v;
								}
							}
						}
					} 
				}
			}
		}
	}
	int res = 0;
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 4; y++) {
			res += dp[(N[0] + N[1] + N[2] + N[3]) & 1][N[1]][N[2]][N[3]][x][y];
			res %= MOD;
		}
	}
	printf("%d\n", res);
	return 0;
}
