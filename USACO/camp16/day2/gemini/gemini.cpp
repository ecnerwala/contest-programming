#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

const int MAXN = 600;
const int MAXM = 600;
const int MAXK = 2e6;

int N, M;
ll K;
ll dp[MAXM][MAXM];

ll cost[MAXM];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K;
	for(int i = 0; i < M; i++) {
		for(int j = 0; j < M; j++) {
			dp[i][j] = 0;
		}
	}
	for(int t = 0; t < N; t++) {
		for(int i = 0; i < M; i++) {
			cin >> cost[i];
		}
		// transition

		ll best;
		// rows first
		for(int i = 0; i < M; i++) {
			best = INF;
			for(int j = 0; j < M; j++) {
				best += K;
				dp[i][j] = best = min(best, dp[i][j]);
			}
			best = INF;
			for(int j = M - 1; j >= 0; j--) {
				best += K;
				dp[i][j] = best = min(best, dp[i][j]);
			}
		}

		// now cols
		for(int i = 0; i < M; i++) {
			best = INF;
			for(int j = 0; j < M; j++) {
				best += K;
				dp[j][i] = best = min(best, dp[j][i]);
			}
			best = INF;
			for(int j = M - 1; j >= 0; j--) {
				best += K;
				dp[j][i] = best = min(best, dp[j][i]);
			}
		}

		// update with costs
		for(int i = 0; i < M; i++) {
			for(int j = 0; j < M; j++) {
				dp[i][j] += cost[i] + cost[j];
			}
		}
		for(int i = 0; i < M; i++) {
			dp[i][i] = INF;
		}
	}
	ll res = INF;
	for(int i = 0; i < M; i++) {
		for(int j = 0; j < M; j++) {
			res = min(res, dp[i][j]);
		}
	}
	cout << res << '\n';
	return 0;
}
