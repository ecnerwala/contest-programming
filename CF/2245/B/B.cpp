#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, C; std::cin >> N >> C;
		std::vector<int64_t> A(N);
		for (auto& a : A) { std::cin >> a; a -= C; }
		std::nth_element(A.begin(), A.begin() + N / 2, A.end());
		int64_t ans = std::accumulate(A.begin() + N / 2, A.end(), int64_t(0));
		for (int i = 0; i < N/2; i++) {
			ans += std::max<int64_t>(A[i], 0);
		}
		std::cout << ans << '\n';
	}

	return 0;
}
