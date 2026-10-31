#include<bits/stdc++.h>
using namespace std;

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T;cin >> T;
	for (int t = 1; t <= T; t++) {
		cout << "Case #" << t << ": ";

		const int MOD = 1013;
		int C; cin >> C;
		string S; cin >> S;
		int N = int(S.size());
		vector<vector<i64>> dp(N+1, vector<i64>(MOD));
		vector<vector<pair<char, int>>> prv(N+1, vector<pair<char, int>>(MOD));
		dp[0][0] = 1;
		for (int i = 0; i < N; i++) {
			for (char c = 'a'; c <= 'z'; c++) {
				if (S[i] == '?' || S[i] == c) {
					for (int v = 0; v < MOD; v++) {
						if (dp[i][v]) {
							dp[i+1][(31*v+c)%MOD] += dp[i][v];
							prv[i+1][(31*v+c)%MOD] = {c, v};
						}
					}
				}
			}
		}
		if (dp[N][C] == 1) {
			string res;
			int v = C;
			for (int i = N; i > 0; i--) {
				char c; tie(c, v) = prv[i][v];
				res += c;
			}
			reverse(res.begin(), res.end());
			cout << res << '\n';
		} else {
			cout << dp[N][C] << '\n';
		}
	}

	return 0;
}
