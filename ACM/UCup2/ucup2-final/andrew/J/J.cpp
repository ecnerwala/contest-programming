#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASE; cin >> CASE;
	while (CASE--) {
		std::string S, T; cin >> S >> T;
		int N = int(S.size());
		int M = int(T.size());
		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = pref[i] + (S[i] == '1' ? +1 : -1);
		}
		std::vector<std::pair<int, int>> dp(M+1);
		std::vector<int> prev(N+1, -1);
		// inclusive range
		int j_lo = 0;
		int j_hi = 0;
		dp[0] = {pref[0], 0};
		prev[0] = 0;
		for (int i = 1; i <= N; i++) {
			int v = pref[i];
			// insert v
			int mi = j_lo - 1, ma = j_hi + 1;
			char dir = T[j_lo];
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if ((dir == '1') ? (v > dp[md].first) : (v < dp[md].first)) {
					mi = md;
				} else {
					ma = md;
				}
			}
			if (mi == -1) {
				// do nothing
				prev[i] = -1;
				continue;
			}
			prev[i] = dp[mi].second;

			dp[ma] = {v, i};
			if (ma == j_hi + 1 && ma < M) {
				j_hi++;
				if (T[j_hi] != T[j_lo]) j_lo = j_hi;
			}
		}
		if (prev[N] == -1) {
			cout << "No" << '\n';
		} else {
			std::string res(N, '0');
			for (int cur = N; cur != 0; cur = prev[cur]) {
				res[cur-1] = '1';
			}
			cout << "Yes" << '\n';
			cout << res << '\n';
		}
	}

	return 0;
}
