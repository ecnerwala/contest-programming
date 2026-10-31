#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
char S[MAXN];
int dp[MAXN][3];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> S;
	N = int(strlen(S));
	memset(dp, -1, sizeof(dp));
	dp[0][1] = dp[0][2] = 0;
	for (int i = 1; i <= N; i++) {
		if (i >= 2) {
			if (dp[i-2][1] != -1) {
				dp[i][2] = max(dp[i][2], dp[i-2][1] + 1);
			}
			if (dp[i-2][2] != -1) {
				if (i >= 4 && S[i-1] == S[i-3] && S[i-2] == S[i-4]) {
					// collision
				} else {
					dp[i][2] = max(dp[i][2], dp[i-2][2] + 1);
				}
			}
		}
		if (dp[i-1][2] != -1) {
			dp[i][1] = max(dp[i][1], dp[i-1][2] + 1);
		}
		if (dp[i-1][1] != -1) {
			if (i >= 2 && S[i-1] == S[i-2]) {
				// collision
			} else {
				dp[i][1] = max(dp[i][1], dp[i-1][1] + 1);
			}
		}
	}

	cout << max(dp[N][1], dp[N][2]) << '\n';

	return 0;
}
