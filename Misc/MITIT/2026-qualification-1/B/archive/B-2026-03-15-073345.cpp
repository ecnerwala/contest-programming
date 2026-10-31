#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		auto hi = *std::max_element(A.begin(), A.end());
		auto lo = *std::max_element(A.begin(), A.end());
		int64_t ans = 0;
		for (auto a : A) {
			ans += std::max(hi - a, a - lo);
		}
		ans -= hi - lo;
		std::cout << ans << '\n';
	}

	return 0;
}
