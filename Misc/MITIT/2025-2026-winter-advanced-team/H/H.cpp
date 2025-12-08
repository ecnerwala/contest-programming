#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		int prv_r = 0;
		std::array<int, 2> dp{0, 0};
		const int INF = 1e9;
		for (int i = 0; i < N; i++) {
			if (A[i] == i) {
				int l = i; while (l-1 >= 0 && A[l-1] == i) l--;
				int r = i; while (r < N && A[r] == i) r++;
				std::array<int, 2> ndp{INF, INF};
				// try to move left
				if (l > 0) {
					ndp[0] = (i-l+1) + std::min(dp[0], dp[1] - (prv_r == l));
				}
				if (r < N) {
					ndp[1] = (r-i) + std::min(dp[0], dp[1]);
				}
				dp = ndp;
				prv_r = r;
				i = r-1;
			}
		}
		int ans = std::min(dp[0], dp[1]);
		if (ans == INF) {
			ans = -1;
		}
		cout << ans << '\n';
	}

	return 0;
}
