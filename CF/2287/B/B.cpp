#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::vector<int64_t> pref(N+1);
		for (int i = 0; i < N; i++) pref[i+1] = pref[i] + A[i];

		int64_t ans = pref[N];

		int lo = 0, hi = N-1;
		int64_t cost_1 = A[lo] + A[hi];
		while (hi - lo > 1) {
			ans = std::min(ans, std::max(cost_1, ));
		}
	}

	return 0;
}

// If train 1 doesn't stop at station X, then train 2 must take X <-> everything
