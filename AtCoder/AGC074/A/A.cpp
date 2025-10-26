#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [x, y] : edges) {
			cin >> x >> y; x--, y--;
		}
		{
			std::vector<int> P(N);
			for (auto& p : P) { cin >> p; p--; }
			for (auto& [x, y] : edges) { x = P[x], y = P[y]; assert(x < y); }
		}
		std::vector<int> nxt(N, N);
		std::vector<int> prv(N, -1);
		for (auto& [x, y] : edges) {
			prv[y] = std::max(prv[y], x);
			nxt[x] = std::min(nxt[x], y);
		}
		std::vector<int> dp(N);
		for (int i = 0; i < N; i++) {
			if (i+1 < N) {
				dp[i+1] = std::max(dp[i+1], dp[i]);
			}
			if (prv[i] >= 0) {
				dp[i] = std::max(dp[i], dp[prv[i]]);
			}
			dp[i]++;
			if (nxt[i] < N) {
				dp[nxt[i]] = std::max(dp[nxt[i]], dp[i]);
			}
			if (i-1 >= 0) dp[i] = std::max(dp[i], dp[i-1]);
		}
		cout << N - dp[N-1] << '\n';
	}

	return 0;
}
