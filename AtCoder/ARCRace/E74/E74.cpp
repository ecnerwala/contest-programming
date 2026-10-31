#include<bits/stdc++.h>
using namespace std;

const int MOD = int(1e9)+7;

const int MAXN = 400;
const int MAXM = 400;
int N, M;
vector<int> constraints[MAXN][4];
int dp[MAXN][MAXN][MAXN]; // decreasing

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int l, r, x; cin >> l >> r >> x;
		constraints[r][x].push_back(l);
	}
	for (int r = 1; r <= N; r++) {
		for (int x = 1; x <= 3; x++) {
			sort(constraints[r][x].begin(), constraints[r][x].end());
		}
	}
	dp[0][0][0] = 1;
	int ans = 0;
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= i; j++) {
			for (int k = 0; k <= j; k++) {
				if (!constraints[i][1].empty() && j >= constraints[i][1].front()) {
					continue;
				}
				if (!constraints[i][2].empty() && j < constraints[i][2].back()) {
					continue;
				}
				if (!constraints[i][2].empty() && k >= constraints[i][2].front()) {
					continue;
				}
				if (!constraints[i][3].empty() && k < constraints[i][3].back()) {
					continue;
				}

				if (i == N) {
					ans += dp[i][j][k];
					if (ans >= MOD) ans -= MOD;
				} else {
					dp[i+1][j][k] += dp[i][j][k];
					if (dp[i+1][j][k] >= MOD) dp[i+1][j][k] -= MOD;
					dp[i+1][i][k] += dp[i][j][k];
					if (dp[i+1][i][k] >= MOD) dp[i+1][i][k] -= MOD;
					dp[i+1][i][j] += dp[i][j][k];
					if (dp[i+1][i][j] >= MOD) dp[i+1][i][j] -= MOD;
				}
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
