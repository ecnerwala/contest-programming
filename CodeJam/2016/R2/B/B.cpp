#include<bits/stdc++.h>
using namespace std;

typedef double ld;

const int MAXN = 400;
const int MAXK = MAXN;
int N, K;
ld P[2][MAXN];
ld dp[2][MAXN][MAXK]; // using the first i, probability of exactly j

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> N >> K;
		for(int i = 0; i < N; i++) {
			ld v;
			cin >> v;
			P[0][i] = P[1][i] = v;
		}
		sort(P[0], P[0] + N);
		sort(P[1], P[1] + N);
		reverse(P[1], P[1] + N);
		memset(dp, 0, sizeof(dp));
		for(int z = 0; z < 2; z++) {
			dp[z][0][0] = 1;
			for(int i = 0; i < N; i++) {
				for(int j = 0; j <= i; j++) {
					//cerr << z << ' ' << i << ' ' << j << ' ' << dp[z][i][j] << '\n';
					dp[z][i + 1][j + 1] += dp[z][i][j] * P[z][i];
					dp[z][i + 1][j + 0] += dp[z][i][j] * (1 - P[z][i]);
				}
			}
		}

		ld res = 0;
		for(int l = 0; l <= K; l++) {
			int r = K - l;
			ld val = 0;
			for(int i = 0; i <= K / 2; i++) {
				val += dp[0][l][i] * dp[1][r][K / 2 - i];
			}
			//cerr << l << ' ' << r << ' ' << val << '\n';
			res = max(res, val);
		}
		
		cout << "Case #" << case_num << ": ";
		cout << fixed << setprecision(9) << res << '\n';
	}

	return 0;
}
