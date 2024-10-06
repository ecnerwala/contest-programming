#include <bits/stdc++.h>

template <typename T, typename U> void setmax(T& a, U b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		const int64_t INF = int64_t(1e18);

		int NUM_DAYS, M; cin >> NUM_DAYS >> M;
		std::vector<int64_t> A(M);
		std::vector<int64_t> dp(M+1, -INF), ndp(M+1);
		for (int r = 0; r < NUM_DAYS; r++) {
			for (auto& a : A) cin >> a;

			std::fill(ndp.begin(), ndp.end(), -INF);
			for (int z = 0; z < 2; z++) {
				int64_t ways_not = -INF;
				int64_t ways_use = -INF;
				for (int i = 0; i < M; i++) {
					setmax(ways_use, dp[i] + ways_not);
					if (r == 0) setmax(ways_use, 0);
					setmax(ways_not, 0);
					ways_not += A[i];
					ways_use += A[i];
					setmax(ndp[i+1], ways_use);
				}
				std::reverse(A.begin(), A.end());
				std::reverse(dp.begin(), dp.end());
				std::reverse(ndp.begin(), ndp.end());
			}
			std::swap(dp, ndp);
		}
		cout << *std::max_element(dp.begin(), dp.end()) << '\n';
	}

	return 0;
}
