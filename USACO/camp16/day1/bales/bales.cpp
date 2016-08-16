#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

// dp[i][v] is min weight for value v, INF for impossible

const int MAXV = 1e3 + 10;
const int MAXN = 100;

int N;
ll M;
int W[MAXN];
int V[MAXN];
int P[MAXN];

ll dp[MAXN][MAXV];

ll ndp[MAXV];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 1; i <= N; i++) {
		cin >> W[i] >> V[i] >> P[i];
		assert(P[i] < i);
	}
	for(int i = 0; i <= N; i++) {
		dp[i][0] = 0;
		for(int j = 1; j < MAXV; j++) {
			dp[i][j] = M + 1;
		}
	}

	for(int i = N; i > 0; i--) {
		int p = P[i];
		memcpy(ndp, dp[p], sizeof(ndp));
		for(int v = 0; v < MAXV; v++) {
			if(dp[i][v] > M) continue;
			for(int n = v + V[i]; n < MAXV; n++) {
				ndp[n] = min(ndp[n], dp[p][n - v - V[i]] + dp[i][v] + W[i]);
			}
		}
		memcpy(dp[p], ndp, sizeof(ndp));
	}
	int res = 0;
	for(int i = 0; i < MAXV; i++) {
		if(dp[0][i] <= M) res = i;
	}
	cout << res << '\n';

	return 0;
}
