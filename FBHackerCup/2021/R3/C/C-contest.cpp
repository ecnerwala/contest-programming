#include<bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		std::vector<int> P(N);
		std::vector<std::vector<int>> ch(N);
		for (int i = 1; i < N; i++) {
			cin >> P[i]; P[i]--;
			ch[P[i]].push_back(i);
		}

		assert(K >= 2);
		std::vector<int> sz(N);
		std::vector<int> dp(N);
		for (int i = N-1; i >= 0; i--) {
			for (int c : ch[i]) sz[i] += 1 + sz[c];

			// our options:
			{ // Do nothing, just take as many children
				int v = 0;
				for (int c : ch[i]) v += dp[c];
				setmax(dp[i], v);
			}

			{ // Take 1 large child, we can then compute him
				for (int c : ch[i]) {
					if (sz[c] >= K) {
						setmax(dp[i], dp[c] + 1);
					}
				}
			}
			{ // Take at least K children, one of which has 1 child (which we add), the rest of which have 0 children or at least K
				int v = 0;
				bool has_extra = false;
				int cnt = 0;
				for (int c : ch[i]) {
					cnt++;
					if (dp[c] > 0) {
						assert(sz[c] >= K);
						v += dp[c];
					} else if (sz[c] > 0) {
						has_extra = true;
					}
				}
				if (cnt >= K && has_extra) {
					setmax(dp[i], v+1);
				}
			}
		}

		cout << "Case #" << case_num << ": " << dp[0] << '\n';
	}

	return 0;
}
