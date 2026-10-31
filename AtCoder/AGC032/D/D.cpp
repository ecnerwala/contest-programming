#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXN = 5100;
int N;
ll dp[MAXN][MAXN];
int P[MAXN];
ll A, B;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> N >> A >> B;
	for (int i = 1; i <= N; i++) {
		cin >> P[i];
	}
	P[N+1] = N+1;

	dp[0][0] = 0;
	for (int i = 1; i <= N+1; i++) {
		dp[i][i] = ll(1e18);
		for (int j = 0; j < i; j++) {
			if (P[i] > P[j]) dp[i][i] = min(dp[i][i], dp[i-1][j]);
			dp[i][j] = dp[i-1][j] + (P[i] > P[j] ? A : B);
		}
	}

	cout << dp[N+1][N+1] << '\n';

	return 0;
}
