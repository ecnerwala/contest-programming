#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N); for (auto& v : A) { cin >> v; v--; }
		std::vector<int> cold(K); for (auto& v : cold) cin >> v;
		std::vector<int> hot(K); for (auto& v : hot) cin >> v;

		const int64_t INF = 1e18;
		std::vector<int64_t> dp(K+1, INF);
		dp[K] = 0;
		std::vector<int64_t> ndp(K+1);
		int prv_color = K;
		for (int nxt_color : A) {
			for (int o = 0; o <= K; o++) {
				ndp[o] = dp[o] + (nxt_color == prv_color ? hot[nxt_color] : cold[nxt_color]);
				if (o == prv_color) {
					for (int x = 0; x <= K; x++) {
						ndp[o] = std::min(ndp[o], dp[x] + (x == nxt_color ? hot[nxt_color] : cold[nxt_color]));
					}
				}
			}
			std::swap(dp, ndp);
			prv_color = nxt_color;
		}
		cout << *std::min_element(dp.begin(), dp.end()) << '\n';
	}

	return 0;
}
