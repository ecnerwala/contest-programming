#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, S; cin >> N >> S;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::pair<int64_t, int64_t>> dp; dp.reserve(2);
		std::vector<std::pair<int64_t, int64_t>> ndp; ndp.reserve(2);
		dp.push_back({A[0], 0});

		for (int i = 1; i < N; i++) {
			int64_t a = A[i];
			ndp.clear();
			auto check = [&](int64_t v) {
				assert(v >= 0);
				int64_t r = dp[0].first * v + dp[0].second;
				for (auto [x, y] : dp) {
					r = std::min(r, x * v + y);
				}
				ndp.push_back({a - v, r});
			};
			if (i == N-1) {
				check(a);
			} else if (a == 2 * S) {
				check(S);
			} else if (a > 2 * S) {
				check(S);
				check(a - S);
			} else if (a >= S) {
				check(S);
				check(a - S);
			} else {
				check(0);
				check(a);
			}
			std::swap(dp, ndp);
		}

		cout << dp.back().second << '\n';
	}

	return 0;
}
