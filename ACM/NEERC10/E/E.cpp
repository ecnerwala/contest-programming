#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

const int MAXN = 4500;
int N, M;
pair<ll, int> X[MAXN];
const int MAXM = MAXN;
pair<ll, int> Y[MAXM];

ll dp[MAXN][MAXM];
int prv[MAXN][MAXM];

int main() {
	if (fopen("evacuation.in", "r")) {
		freopen("evacuation.in", "r", stdin);
		freopen("evacuation.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> X[i].first; X[i].second = i;
	}
	cin >> M;
	for (int j = 0; j < M; j++) {
		cin >> Y[j].first; Y[j].second = j;
	}

	sort(X, X + N);
	sort(Y, Y + M);

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= M; j++) {
			dp[i][j] = INF;
		}
	}
	dp[1][0] = abs(X[0].first - Y[0].first);
	for (int i = 1; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (dp[i][j] == INF) continue;
			ll cnd = dp[i][j] + abs(X[i].first - Y[j].first);
			if (cnd < dp[i+1][j]) {
				dp[i+1][j] = cnd;
				prv[i+1][j] = j;
			}
			if (j+1 < M) {
				cnd = dp[i][j] + abs(X[i].first - Y[j+1].first);
				if (cnd < dp[i+1][j+1]) {
					dp[i+1][j+1] = cnd;
					prv[i+1][j+1] = j;
				}
			}
		}
	}
	cout << dp[N][M-1] << '\n';
	vector<int> res(N);
	for (int i = N, j = M-1; i >= 1; i--) {
		res[X[i-1].second] = Y[j].second;
		j = prv[i][j];
	}
	for (int i = 0; i < N; i++) {
		cout << res[i]+1 << " \n"[i==N-1];
	}

	return 0;
}
