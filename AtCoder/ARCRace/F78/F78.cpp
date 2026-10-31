#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXN = 15;
int N;
int W[MAXN][MAXN];

int V[1<<MAXN];
int dp[1<<MAXN][MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int M;
	cin >> N >> M;
	int tot = 0;
	for (int i = 0; i < M; i++) {
		int a, b, c; cin >> a >> b >> c; a--, b--;
		W[a][b] = W[b][a] = c;
		tot += c;
	}

	for (int m = 0; m < (1<<N); m++) {
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if ((m & (1 << i)) && (m & (1 << j))) {
					V[m] += W[i][j];
				}
			}
		}
	}

	for (int m = 1; m < (1 << N); m += 2) {
		dp[m][0] = V[m];
		for (int i = 1; i < N; i++) {
			dp[m][i] = -INF;
		}
		for (int v = (m-2) & m; true; v = (v-2) & m) {
			assert(v & 1);
			assert((m&v) == v);
			for (int i = 1; i < N; i++) {
				if (!((m^v) & (1 << i))) continue;
				for (int j = 0; j < N; j++) {
					if (!(v & (1 << j))) continue;
					if (W[i][j] == 0) continue;
					dp[m][i] = max(dp[m][i], dp[v][j] + V[m^v] + W[i][j]);
				}
			}
			if (v == 1) break;
		}
	}
	cout << tot - dp[(1<<N)-1][N-1] << '\n';

	return 0;
}
