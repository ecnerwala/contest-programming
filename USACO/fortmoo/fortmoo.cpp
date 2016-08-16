#include<bits/stdc++.h>
using namespace std;

const int MAXN = 300;
const int MAXM = 300;
int N;
int M;
char grid[MAXM];
int dp[MAXM][MAXM];

int main() {
	if(fopen("fortmoo.in", "r")) {
		freopen("fortmoo.in", "r", stdin);
		freopen("fortmoo.out", "w", stdout);
	}

	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		for(int j = i; j < M; j++) {
			dp[i][j] = -1;
		}
	}

	int res = 0;
	for(int i = 0; i < N; i++) {
		cin >> grid;
		for(int l = 0; l < M; l++) {
			bool row = true;
			for(int r = l; r < M; r++) {
				row = row & (grid[r] == '.');
				if(row) {
					if(dp[l][r] == -1) dp[l][r] = i;
					res = max(res, (r - l + 1) * (i - dp[l][r] + 1));
				} else if(grid[l] == '.' && grid[r] == '.') {
					// do nothing
				} else {
					dp[l][r] = -1;
				}
			}
		}
	}
	cout << res << '\n';

	return 0;
}
