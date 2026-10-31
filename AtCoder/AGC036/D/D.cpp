#include<bits/stdc++.h>
using namespace std;

// the distance functions are in blocks
// forward edges only to future blocks
// backwards edges only to self & immediate predecessor

using ll = long long;

const int MAXN = 600;
int N;
ll A[MAXN][MAXN];

ll backPref[MAXN][MAXN];
ll forwPref[MAXN][MAXN];

ll dp[MAXN][MAXN]; // max value we can save

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (j == i) A[i][j] = 0;
			else cin >> A[i][j];
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			if (i < j) {
				forwPref[i+1][j+1] = A[i][j];
			} else {
				backPref[i+1][j+1] = A[i][j];
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			forwPref[i+1][j+1] += forwPref[i][j+1] + forwPref[i+1][j] - forwPref[i][j];
			backPref[i+1][j+1] += backPref[i][j+1] + backPref[i+1][j] - backPref[i][j];
		}
	}

	for (int j = 1; j < N; j++) {
		// back [0,j) by [0,j)
		// no forwards
		dp[0][j] = backPref[j][j];
	}
	for (int i = 1; i < N; i++) {
		for (int j = i+1; j <= N; j++) {
			for (int k = 0; k < i; k++) {
				// back [i,j) by [k,j)
				// forwards [0,i) by [i,j)
				dp[i][j] = max(dp[i][j], dp[k][i]
						+ (backPref[j][j] - backPref[i][j] - backPref[j][k] + backPref[i][k])
						+ (forwPref[i][j] - forwPref[0][j] - forwPref[i][i] + forwPref[0][i])
						);
			}
		}
	}

	ll ans = 0;
	for (int i = 0; i < N; i++) {
		ans = max(ans, dp[i][N]);
	}

	ans = -ans;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			ans += A[i][j];
		}
	}
	cout << ans << '\n';

	return 0;
}
