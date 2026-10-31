#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		int ans = A[0];
		for (int i = 0; i+1 < N; i++) {
			int a = A[i], b = A[i+1];
			if (a == b) continue;
			int d = 31 - __builtin_clz(a ^ b);
			ans = std::max(ans, a | ((1 << (d+1)) - 1));
		}
		std::cout << ans << '\n';
	}

	return 0;
}
