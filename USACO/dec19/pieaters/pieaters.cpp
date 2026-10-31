#include<bits/stdc++.h>
using namespace std;

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

using ll = long long;

const int MAXN = 310;
const int MAXM = MAXN * MAXN;
int N, M;

int maxCow[MAXN][MAXN][MAXN];

ll dp[MAXN][MAXN];

int main() {
	if (fopen("pieaters.in", "r")) {
		freopen("pieaters.in", "r", stdin);
		freopen("pieaters.out", "w", stdout);
	}

	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int w, l, r; cin >> w >> l >> r; l--;
		for (int z = l; z < r; z++) {
			setmax(maxCow[l][r][z], w);
		}
	}
	for (int d = 1; d <= N; d++) {
		for (int i = 0, j = d; j <= N; i++, j++) {
			for (int k = i; k < j; k++) {
				setmax(maxCow[i][j][k], maxCow[i][j-1][k]);
				setmax(maxCow[i][j][k], maxCow[i+1][j][k]);
				setmax(dp[i][j], dp[i][k] + maxCow[i][j][k] + dp[k+1][j]);
			}
		}
	}
	cout << dp[0][N] << '\n' << flush;

	return 0;
}
