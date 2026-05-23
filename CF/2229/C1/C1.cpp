#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		int64_t ans = 0;
		for (int i = 0; i < N; i++) ans += std::abs(A[i]);
		std::cout << -ans << '\n';
	}

	return 0;
}
