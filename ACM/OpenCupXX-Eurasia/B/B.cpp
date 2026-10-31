#include<bits/stdc++.h>
using namespace std;

using ld = double;

const ld INF = 1e99;
ld sq(ld a) { return a * a; }

const int MAXN = 310;
const int MAXM = 310;
int N;
int M;

ld dist[MAXN][MAXM];

ld H;
ld X1[MAXN];
ld Y1[MAXN];
ld X2[MAXM];
ld Y2[MAXM];

ld dp[MAXN][MAXM];
uint8_t prv[MAXN][MAXN][MAXM];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> H;
	for (int i = 0; i < N; i++) {
		cin >> X1[i] >> Y1[i];
	}
	for (int j = 0; j < M; j++) {
		cin >> X2[j] >> Y2[j];
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			dist[i][j] = sqrt(sq(X1[i] - X2[j]) + sq(Y1[i] - Y2[j]) + sq(H));
		}
	}

	ld ans = INF;
	int bestS = -1;
	for (int s = 0; s < M; s++) {
		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= M; j++) {
				dp[i][j] = INF;
			}
		}
		dp[0][0] = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j <= M; j++) {
				dp[i][j] += dist[i][(j+s)%M];
				for (uint8_t d = 1; d <= 3; d++) {
					int ni = i + ((d >> 0) & 1), nj = j + ((d >> 1) & 1);
					if (ni <= N && nj <= M) {
						if (dp[i][j] < dp[ni][nj]) {
							dp[ni][nj] = dp[i][j];
							prv[s][ni][nj] = d;
						}
					}
				}
			}
		}
		if (dp[N][M] < ans) {
			ans = dp[N][M];
			bestS = s;
		}
	}
	vector<pair<int, int>> segs;
	for (int i = N, j = M; ;) {
		if (i < N) {
			segs.emplace_back(i, (j + bestS) % M);
		}
		if (i == 0 && j == 0) break;
		uint8_t d = prv[bestS][i][j];
		i -= ((d >> 0) & 1), j -= ((d >> 1) & 1);
	}
	reverse(segs.begin(), segs.end());
	cout << fixed << setprecision(15);
	cout << ans << ' ' << int(segs.size()) << '\n';
	for (auto it : segs) {
		cout << it.first+1 << ' ' << it.second+1 << '\n';
	}

	return 0;
}
