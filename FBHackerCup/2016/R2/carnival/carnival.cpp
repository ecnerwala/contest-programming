#include<bits/stdc++.h>
using namespace std;

typedef long double ld;

const int MAXN = 4e3;
const int MAXK = 4e3;
int N, K;
ld P;
ld prob[MAXN][MAXN];
ld dp[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		cin >> N >> K >> P;
		assert(N < MAXN);
		assert(K < MAXK);
		memset(prob, 0, sizeof(prob));
		prob[0][0] = 1;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j <= i; j++) {
				prob[i + 1][j] += (1 - P) * prob[i][j];
				prob[i + 1][j + 1] += P * prob[i][j];
			}
		}
		memset(dp, 0, sizeof(dp));
		for(int i = 1; i <= N; i++) {
			ld val = 0;
			for(int j = K; j <= i; j++) {
				val += prob[i][j];
			}
			for(int v = i; v <= N; v++) {
				dp[v] = max(dp[v], dp[v - i] + val);
			}
		}
		cout << "Case #" << T << ": ";
		cout << fixed << setprecision(20) << dp[N] << '\n';
	}
	return 0;
}
