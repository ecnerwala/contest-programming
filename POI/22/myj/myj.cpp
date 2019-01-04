#include <bits/stdc++.h>
using namespace std;

const int MAXN = 60;
int N;
const int MAXM = 5000;
int M;
pair<long long, pair<int, int>> customers[MAXM];

long long dp[MAXN][MAXN][MAXM];
int nxt[MAXN][MAXN][MAXM];

int A[MAXN];

void build(int l, int r, int m) {
	if (!(l <= r)) return;
	if (dp[l][r][m] == 0) {
		for (int i = l; i <= r; i++) {
			A[i] = customers[M-1].first;
		}
	} else if (nxt[l][r][m] == 0) {
		build(l, r, m+1);
	} else {
		int k = nxt[l][r][m];
		assert(l <= k && k <= r);
		assert(m < M);
		A[k] = customers[m].first;
		build(l, k-1, m);
		build(k+1, r, m);
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	scanf("%d %d", &N, &M);
	for (int i = 0; i < M; i++) {
		int a,b,c; scanf("%d %d %d", &a, &b, &c);
		customers[i] = {c, {a, b}};
	}
	sort(customers, customers + M);
	for (int d = 1; d <= N; d++) {
		for (int l = 1, r = d; r <= N; l++, r++) {
			for (int k = l; k <= r; k++) {
				int cnt = 0;
				for (int m = M-1; m >= 0; m--) {
					if (l <= customers[m].second.first && customers[m].second.second <= r) {
						// we can include it
						if (customers[m].second.first <= k && k <= customers[m].second.second) {
							cnt ++;
							// it crosses
						}
					}
					long long cnd = dp[l][k-1][m] + dp[k+1][r][m] + cnt * customers[m].first;
					if (cnd > dp[l][r][m]) {
						dp[l][r][m] = cnd;
						nxt[l][r][m] = k;
					}
				}
			}
			for (int m = M-2; m >= 0; m--) {
				long long cnd = dp[l][r][m+1];
				if (cnd > dp[l][r][m]) {
					dp[l][r][m] = cnd;
					nxt[l][r][m] = false;
				}
			}
			for (int m = 0; m < M; m++) {
				//cout << l << '-' << r << ' ' << m << ' ' << dp[l][r][m] << ' ' << nxt[l][r][m] << '\n';
			}
		}
	}
	build(1, N, 0);
	cout << dp[1][N][0] << '\n';
	for (int i = 1; i <= N; i++) {
		cout << A[i] << " \n"[i==N];
	}

	return 0;
}
