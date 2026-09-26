#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		int64_t ans = 0;
		if (N >= 2 * K) {
			for (int i = K; i < N-K; i++) ans += A[i];
		}
		for (int i = 0; i < N-1-i && i < N-K+1 && i < K; i++) {
			ans += std::max(A[i], A[N-1-i]);
		}
		std::cout << ans << '\n';
	}

	return 0;
}
