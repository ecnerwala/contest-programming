#include<bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

const int MAXN = 30;
const int MAXM = 60;
const int MAXS = 30;
const int MAXT = 30;
const int MAXC = MAXN * 3;
const int MAXK = 4;

int N;
int S[MAXN];

int M;
int K;
int T[MAXM][MAXK];

int dp[MAXC][MAXT][MAXT][MAXT];
const int COST[8] = {0, 2, 2, 2, 1, 3, 1, 1};

int main() {
	int dumb;
	dumb = scanf("%d", &N);
	int C = N * 3;

	dp[0][0][0][0] = 1;
	for(int s = 0; s < N; s++) {
		int v; dumb = scanf("%d", &v);
		for(int c = C; c > 0; c--) {
			for(int m = 1; m < 8; m++) {
				int nc = c - COST[m];
				if(nc < 0) continue;
				for(int x = (m & 1) ? v : 0, nx = 0; x < MAXT; x++, nx++) {
					for(int y = (m & 2) ? v : 0, ny = 0; y < MAXT; y++, ny++) {
						for(int z = (m & 4) ? v : 0, nz = 0; z < MAXT; z++, nz++) {
							dp[c][x][y][z] += dp[nc][nx][ny][nz];
							if(dp[c][x][y][z] >= MOD) dp[c][x][y][z] -= MOD;
						}
					}
				}
			}
		}
	}

	dumb = scanf("%d%d", &M, &K);
	if(K == 3) {
		for(int i = 0; i < M; i++) {
			int x, y, z;
			dumb = scanf("%d%d%d", &x, &y, &z);
			for(int c = 1; c <= C; c++) {
				printf("%d%c", dp[c][x][y][z], (c == C ? '\n' : ' '));
			}
		}
	} else if(K == 2) {
		for(int i = 0; i < M; i++) {
			int x, y;
			dumb = scanf("%d%d", &x, &y);
			for(int c = 1; c <= C; c++) {
				printf("%d%c", dp[c][0][x][y], (c == C ? '\n' : ' '));
			}
		}
	}
	return dumb - dumb;
}
