#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int X, Y; cin >> X >> Y;
		string S; cin >> S;

		const int INF = 1e9;
		array<int, 2> dp{0,0};
		for (int i = 0; i < int(S.size()); i++) {
			if (i) {
				dp = {min(dp[0], dp[1] + Y), min(dp[1], dp[0] + X)};
			}
			char c = S[i];
			if (c == 'C') dp[1] = INF;
			else if (c == 'J') dp[0] = INF;
		}

		int ans = min(dp[0], dp[1]);
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
