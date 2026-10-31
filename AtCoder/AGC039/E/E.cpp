#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 44;
int N;
bool A[MAXN*2][MAXN*2];

ll go2(int a, int b, int c, int d);

ll dp[MAXN][MAXN][MAXN];
// inclusive range, with b chosen as special
ll go(int a, int b, int c) {
	if (dp[a][b][c] != -1) {
		return dp[a][b][c];
	}
	if (a == b && c == b) {
		return dp[a][b][c] = 1;
	} else if (a == b || c == b) {
		return dp[a][b][c] = 0;
	}

	ll ans = 0;
	for (int p1 = a; p1 <= b-1; p1++) {
		for (int p2 = b+1; p2 <= c; p2++) {
			ans += go2(a, p1, p2, c) * go(p1+1, b, p2-1);
		}
	}
	return dp[a][b][c] = ans;
}

ll dp2[MAXN][MAXN][MAXN][MAXN];
ll go2(int a, int b, int c, int d) {
	if (dp2[a][b][c][d] != -1) {
		return dp2[a][b][c][d];
	}
	if (b-a+1 == 0 && d-c+1 == 0) {
		return dp2[a][b][c][d] = 1;
	} else if (b-a+1 == 0 || d-c+1 == 0) {
		return dp2[a][b][c][d] = 0;
	}

	ll ans = 0;
	for (int x = a; x <= b; x++) {
		for (int y = c; y <= d; y++) {
			ans += A[x][y] * go(a,x,b) * go(c,y,d);
		}
	}
	return dp2[a][b][c][d] = ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N; N *= 2;
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		for (int j = 0; j < N; j++) {
			A[i][j] = bool(s[j] - '0');
		}
	}
	memset(dp, -1, sizeof(dp));
	memset(dp2, -1, sizeof(dp2));
	cout << go2(0,0,1,N-1) << '\n';

	return 0;
}
