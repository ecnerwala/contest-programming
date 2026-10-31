#include <bits/stdc++.h>

int main() {
	static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
	std::mt19937 mt(FIXED_RANDOM);
	int N = std::uniform_int_distribution<int>(1, 20)(mt);
	std::cout << N << ' ' << (1 << std::uniform_int_distribution<int>(0, 10)(mt)) << '\n';
	for (int i = 1; i < N; i++) {
		int a = 1 << std::uniform_int_distribution<int>(0, 10)(mt);
		int u = std::uniform_int_distribution<int>(1, i-1)(mt);
		int v = std::uniform_int_distribution<int>(1, i-1)(mt);
		std::cout << a << ' ' << u << ' ' << v << '\n';
	}
}
