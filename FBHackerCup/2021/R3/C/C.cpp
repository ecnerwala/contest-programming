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
			int v = 0;
			bool has_extra = false;
			for (int c : ch[i]) {
				sz[i] += 1 + sz[c];

				if (sz[c] >= K) {
					setmax(dp[i], dp[c]+1);
				}

				if (dp[c] > 0) {
					v += dp[c];
				} else if (sz[c] > 0) {
					has_extra = true;
				}
			}

			if (int(ch[i].size()) >= K && has_extra) v++;
			setmax(dp[i], v);
		}

		cout << "Case #" << case_num << ": " << dp[0] << '\n';
	}

	return 0;
}
