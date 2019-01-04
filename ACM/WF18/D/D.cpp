#include<bits/stdc++.h>
using namespace std;

const int MAXN = 600;
const int MAXD = 600;

const int MAXV = MAXN + MAXD;
double bin[MAXV][MAXV];

int N, D, R;

double dp[MAXN][MAXD];

int main() {
	bin[0][0] = 1;
	for (int i = 1; i < MAXV; i++) {
		for (int j = 0; j < i; j++) {
			bin[i][j] += bin[i-1][j];
		}
		for (int j = 1; j <= i; j++) {
			bin[i][j] += bin[i-1][j-1];
		}
	}

	scanf("%d %d %d", &N, &D, &R);

	double res = R;

	// Count number of things >= v
	for (int v = 1; v <= D; v++) {
		// i is the number of things < v
		for (int i = 0; i <= N; i++) {
			if (i) {
				dp[i][0] = dp[i-1][0];
				for (int a = 1; a <= D; a++) {
					dp[i][a] = dp[i][a-1] + dp[i-1][a];
					if (a >= v) {
						dp[i][a] -= dp[i-1][a-v];
					}
				}
			} else {
				memset(dp[0], 0, sizeof(dp[0]));
				dp[0][0] = 1;
			}

			for (int a = 0; a + v * (N-i) <= D; a++) {
				// exactly N-i are >= v
				double prob = bin[D-(N-i)*v-a + (N-i)-1][(N-i)-1] * bin[N][i] * dp[i][a];
				prob /= bin[D+N-1][N-1];
				res += prob * min(N-i, R);
			}
		}
	}

	printf("%.10f\n", res);

	return 0;
}
