#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
typedef pair<pii, pii> ppp;
#define A first
#define B second

const int INF = 1e8;

const int MAXN = 400;
const int MAXM = 10000;
const int MAXE = 400;

int N, M, E;

ppp edges[MAXM];

int dp[MAXN][MAXE];

int main() {
	cin >> N >> M >> E;
	for(int i = 0; i < M; i++)  {
		cin >> edges[i].A.A >> edges[i].A.B >> edges[i].B.A >> edges[i].B.B;
	}
	for(int i = 1; i <= N; i++) {
		for(int e = 0; e <= E; e++) {
			dp[i][e] = -INF;
		}
	}

	dp[1][0] = 0;

	for(int e = 0; e <= E; e++) {
		for(int m = 0; m < M; m++) {
			int a = edges[m].A.A, b = edges[m].A.B;
			int c = edges[m].B.A, v = edges[m].B.B;
			if(dp[a][e] == -INF) continue;
			if(e + c > E) continue;
			dp[b][e + c] = max(dp[b][e + c], dp[a][e] + v);
		}
	}

	int res = -INF;
	for(int e = 0; e <= E; e++) {
		res = max(res, dp[N][e]);
	}

	if(res == -INF) {
		cout << "IMPOSSIBLE" << '\n';
	} else {
		cout << res << '\n';
	}
}
