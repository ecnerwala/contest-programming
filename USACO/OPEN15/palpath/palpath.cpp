#include<bits/stdc++.h>
using namespace std;

const int MOD = 1e9 + 7;

const int MAXN = 1e3;

int N;
char S[2][MAXN][MAXN];
int dp[2][MAXN][MAXN];

inline void add(int &a, int b) {
	a += b;
	if(a >= MOD) a -= MOD;
}

int main() {
	if(fopen("palpath.in", "r")) {
		freopen("palpath.in", "r", stdin);
		freopen("palpath.out", "w", stdout);
	}
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%s", S[0][i]);
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			S[1][i][j] = S[0][N - 1 - i][N - 1 - j];
		}
	}

	memset(dp, 0, sizeof(dp));

	dp[0][0][0] = 1; // number of ways of getting here, this doesn't have to be same

	for(int d = 0; d + 1 < N; d++) {
		for(int i = 0; i <= d; i++) {
			for(int j = 0; j <= d; j++) {
				if(S[0][i][d - i] == S[1][j][d - j]) {
					add(dp[(d + 1) % 2][i + 0][j + 0], dp[d % 2][i][j]);
					add(dp[(d + 1) % 2][i + 0][j + 1], dp[d % 2][i][j]);
					add(dp[(d + 1) % 2][i + 1][j + 0], dp[d % 2][i][j]);
					add(dp[(d + 1) % 2][i + 1][j + 1], dp[d % 2][i][j]);
					//cerr << d << ' ' << i << ' ' << j << '\n';
					//cerr << dp[d % 2][i][j] << ' ' << dp[(d + 1) % 2][i][j] << '\n';
				}

				dp[d % 2][i][j] = 0;
			}
		}
	}

	int res = 0;
	for(int i = 0; i < N; i++) {
		res += dp[(N - 1) % 2][i][N - 1 - i];
		if(res >= MOD) res -= MOD;
	}

	printf("%d\n", res);

	return 0;
}
