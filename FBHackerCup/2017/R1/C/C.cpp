#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

const int MAXN = 200;
int N, M;
ll dist[MAXN][MAXN];

const int MAXK = 1e4;
int K;
int P[MAXK + 1][2];
ll dp[MAXK + 1][2];

ll go() {
	dp[0][0] = dist[0][P[0][0]];
	dp[0][1] = dist[0][P[0][1]];
	for (int i = 1; i <= K; i++) {
		dp[i][0] = min(
				dp[i-1][0] + dist[P[i-1][0]][P[i][1]],
				dp[i-1][1] + dist[P[i-1][1]][P[i][1]]
				)
			+ dist[P[i][1]][P[i][0]];
		if (dp[i][0] >= INF) {
			return -1;
		}
		dp[i][1] = min(
				dp[i-1][0] + dist[P[i-1][0]][P[i][0]],
				dp[i-1][1] + dist[P[i-1][1]][P[i][0]]
				)
			+ dist[P[i][0]][P[i][1]];
		if (dp[i][1] >= INF) {
			return -1;
		}
	}
	return dp[K][0];
}

int main() {
	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cout << "Case #" << T << ": ";

		cin >> N >> M >> K;
		assert(N < MAXN);
		assert(K < MAXK);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				dist[i][j] = (i == j) ? 0 : INF;
			}
		}

		for (int i = 0; i < M; i++) {
			int A, B; ll G; cin >> A >> B >> G;
			A--, B--;
			assert(0 <= A && A < N);
			assert(0 <= B && B < N);
			dist[A][B] = min(dist[A][B], G);
			dist[B][A] = min(dist[B][A], G);
		}

		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
				}
			}
		}

		cin >> P[0][0]; P[0][0] --;
		P[0][1] = P[0][0];
		for (int i = 1; i < K; i++) {
			cin >> P[i][0] >> P[i][1]; P[i][0] --, P[i][1] --;
		}
		cin >> P[K][0]; P[K][0] --;
		P[K][1] = P[K][0];

		cout << go() << '\n';
	}

	return 0;
}
