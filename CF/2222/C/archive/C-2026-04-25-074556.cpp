#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;

		int med;
		{
			assert(N & 1);
			auto vals = A;
			std::nth_element(A.begin(), A.begin() + (N/2), A.end());
			med = A[N/2];
		}
		std::vector<int> pref_lt(N+1);
		std::vector<int> pref_le(N+1);
		for (int i = 0; i < N; i++) {
			pref_lt[i+1] = pref_lt[i] + (A[i] < med);
			pref_le[i+1] = pref_le[i] + (A[i] <= med);
		}
		std::vector<int> dp(N+1);
		dp[0] = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j <= N; j += 2) {
				int cnt_lt = pref_lt[j] - pref_lt[i];
				int cnt_le = pref_le[j] - pref_le[i];
				int cnt_tot = j - i;
				if (cnt_lt <= cnt_tot / 2 && cnt_tot / 2 < cnt_le) {
					dp[j] = std::max(dp[j], dp[i] + 1);
				}
			}
		}
		std::cout << dp[N] << '\n';
	}

	return 0;
}
