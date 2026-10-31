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
		int64_t extra = 0;

		int64_t mi = 0;

		int prv_color = K;
		for (int nxt_color : A) {
			if (nxt_color == prv_color) {
				extra += hot[prv_color];
				continue;
			}
			extra += cold[nxt_color];
			mi = dp[prv_color] = std::min({mi, dp[nxt_color] + hot[nxt_color] - cold[nxt_color]});
			prv_color = nxt_color;
		}
		cout << extra + *std::min_element(dp.begin(), dp.end()) << '\n';
	}

	return 0;
}
