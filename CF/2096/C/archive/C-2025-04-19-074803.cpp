#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<bool, 3>> A_bad(N-1, {false, false, false});
		std::vector<std::array<bool, 3>> B_bad(N-1, {false, false, false});
		std::vector<int> prv_row(N, -2);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int h; cin >> h;
				if (i) {
					int diff = h - prv_row[j];
					if (-1 <= diff && diff <= 1) {
						A_bad[i-1][diff+1] = true;
					}
				}
				if (j) {
					int diff = h - prv_row[j-1];
					if (-1 <= diff && diff <= 1) {
						B_bad[j-1][diff+1] = true;
					}
				}
				prv_row[j] = h;
			}
		}
		std::vector<int> A(N);
		std::vector<int> B(N);
		for (auto& a : A) cin >> a;
		for (auto& b : B) cin >> b;

		auto solve = [](const std::vector<int>& C, std::vector<std::array<bool, 3>>& bad) -> std::optional<int64_t> {
			const int64_t INF = 2e18;
			std::array<int64_t, 2> dp({0, C[0]});
			for (int i = 1; i < int(C.size()); i++) {
				std::array<int64_t, 2> ndp({INF, INF});
				for (int p = 0; p <= 1; p++) {
					if (dp[p] == INF) continue;
					for (int n = 0; n <= 1; n++) {
						if (bad[i-1][p-n+1]) continue;
						ndp[n] = std::min(ndp[n], dp[p] + (n * C[i]));
					}
				}
				dp = ndp;
			}
			int64_t res = std::min(dp[0], dp[1]);
			if (res >= INF) return -1;
			return res;
		};
		auto A_ans = solve(A, A_bad);
		auto B_ans = solve(B, B_bad);
		if (A_ans && B_ans) {
			cout << *A_ans + *B_ans << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
