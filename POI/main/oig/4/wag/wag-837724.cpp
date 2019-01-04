#include<bits/stdc++.h>
using namespace std;

const int MAXL = 26;
const int MAXR = 26;
const int MAXN = 51;
const int MAXW = 51;

int N, L, R;
int V;

const int MAXV = MAXW * MAXN * (MAXL + MAXR);

int dp[2][MAXV * 2];

int INF;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> L >> R;
	INF = N * (L + R);
	V = MAXW * N * (L + R);
	for (int i = 0; i < V * 2; i ++) {
		dp[0][i] = -INF - 1;
	}
	dp[0][V] = 0;
	for (int i = 0; i < L + R; i++) {
		int d = i % 2;
		int nd = 1-d;
		memcpy(dp[nd], dp[d], sizeof(dp[d]));

		int s = (i < L) ? 1 : -1;
		int v = 0;

		for (int j = 1; j <= N; j++) {
			int w; cin >> w; w *= s; v += w;
			for (int val = 0; val < V * 2; val ++) {
				if (dp[d][val] < 0) continue;
				dp[nd][val+v] = max(dp[nd][val+v], dp[d][val] + j);
			}
		}
	}

	cout << N * (L + R) - dp[(L+R) % 2][V] << '\n';

	return 0;
}
