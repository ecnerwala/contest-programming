#include<bits/stdc++.h>
using namespace std;

template <typename T> void setmax (T& a, T b) { if (b > a) a = b; }

using ld = double;
using ll = long long;

const int MAXN = 110;
const int MAXK = 110;
const int MAXS = 1.1e6;
int N, K, S; ll P;
ld X[MAXN][3];
ld dp[MAXN][MAXN];
ld ndp[MAXN][MAXN];

ld knap[MAXS];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K >> S >> P;
	for (int i = 0; i < N; i++) {
		cin >> X[i][0] >> X[i][1] >> X[i][2];
		sort(X[i], X[i] + 3);
		reverse(X[i], X[i] + 3);
		X[i][1] += X[i][0];
		X[i][2] = 1;
	}

	dp[0][0] = P;

	for (int i = 0; i < N; i++) {
		memset(ndp, 0, sizeof(ndp));
		for (int a = 0; a <= i; a++) {
			for (int b = 0; a + b <= i; b++) {
				setmax(ndp[a][b], dp[a][b] * X[i][0]);
				setmax(ndp[a+1][b], dp[a][b] * X[i][1]);
				setmax(ndp[a][b+1], dp[a][b]); // * 1
			}
		}
		memcpy(dp, ndp, sizeof(ndp));
	}

	for (int k = 0; k < K; k++) {
		int i, j; cin >> i >> j;
		ld val = dp[i][j];
		int c; cin >> c;
		for (int v = 0; v + c <= S; v++) {
			setmax(knap[v+c], knap[v] + val);
		}
	}
	cout << fixed << setprecision(20) << knap[S] << '\n';

	return 0;
}
