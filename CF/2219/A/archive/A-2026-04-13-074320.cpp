#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int P, Q; std::cin >> P >> Q;
		// P + 2 * Q = (N-1) * M + (M-1) * N = 2 * M * N - M - N = ((2M-1) * (2N-1) - 1) / 2
		int cnd = (P + 2 * Q) * 2 + 1;
		// we also need Q <= N*M - max(N, M)
		[&]() -> void {
			for (int i = 3; i * i <= cnd; i += 2) {
				if (cnd % i == 0) {
					int N = i / 2, M = (cnd / i) / 2;
					if (Q <= N * M - M) {
						std::cout << N << ' ' << M << '\n';
						return;
					}
				}
			}
			std::cout << -1 << '\n';
			return;
		}();
	}

	return 0;
}
