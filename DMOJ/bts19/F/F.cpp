#include<bits/stdc++.h>
using namespace std;

const int MAXN = 6.1e5;
int N;
int H[MAXN];

int dp[MAXN][4][4];

template <typename T> void setmin(T& a, const T& b) {
	if (b < a) a = b;
}

void recurse(int l, int r) {
	if (r - l + 1 <= 3) {
		// no point swapping
		int v = 0;
		if (r - l + 1 == 3) {
			v += (H[l] < H[l+1]) != (H[l+1] < H[r]);
		}
		setmin(dp[l][H[l] < H[l+1]][H[r] < H[r-1]], v);
		return;
	}
	recurse(l+1, r-1);

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			int nx = H[l] < H[l+1];
			int ny = H[r] < H[r-1];
			setmin(dp[l][nx][ny], dp[l+1][x][y] + (nx != x) + (ny != y));
		}
	}
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			int nx = H[l] < H[r-1];
			int ny = H[r] < H[l+1];
			setmin(dp[l][nx][ny], dp[l+1][x][y] + (nx != y) + (ny != x));
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> H[i];
	}
	for (int i = 0; i < N; i++) {
		dp[i][0][0] = dp[i][0][1] = dp[i][1][0] = dp[i][1][1] = N+1;
	}
	recurse(0, N-1);
	cout << min(min(dp[0][0][0], dp[0][0][1]), min(dp[0][1][0], dp[0][1][1])) + 1 << '\n';

	return 0;
}
