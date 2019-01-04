#include<bits/stdc++.h>
using namespace std;

static const int MAXN = 600;
int N;
char S[MAXN];
int dp[MAXN][MAXN][MAXN];
int ndp[MAXN];
int matches[MAXN][MAXN];
bool good[MAXN];

int go() {
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N; j++) {
			matches[i][j] = 0;
			for (int k = 1; k <= min(i,j); k++) {
				if (S[i-k] == S[j-k]) {
					matches[i][j] ++;
				} else {
					break;
				}
			}
		}
	}
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N; j++) {
			for (int k = 0; k <= N; k++) {
				dp[i][j][k] = i+1;
			}
		}
	}
	dp[1][1][1] = 1;
	for (int k = 2; k <= N; k++) {
		for (int r = 1; r < k; r++) {
			for (int len = 1; len <= r; len++) {
			}
		}
	}
	for (int r = 1; r <= N; r++) {
		for (int k = r + 1; k <= N; k++) good[k] = true;

		for (int len = 1; len <= r; len++) {
			for (int k = r+len; k <= N; k++) {
				if (S[r-len] != S[k-len]) good[k] = false;
			}
			for (int k = r; k <= N; k++) {
				ndp[k] = dp[k] + 1;
			}
			for (int k = r+len; k <= N; k++) {
				ndp[k] = min(ndp[k], ndp[k-1]+1);
				if (good[k]) ndp[k] = min(ndp[k], ndp[k-len]+1);
			}
			for (int k = r+len; k <= N; k++) {
				dp[k] = min(dp[k], ndp[k]);
			}
		}
	}
	return dp[N];
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> S;
		N = int(strlen(S));
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
