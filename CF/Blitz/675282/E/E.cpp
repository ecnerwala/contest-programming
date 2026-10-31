#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int64_t N, K; std::cin >> N >> K;
		int64_t a = 0, b = 1 % K, c = 0;
		do {
			b += a; if (b >= K) b -= K;
			std::swap(a, b);
			c++;
		} while (a != 0);
		const int64_t MOD = int(1e9) + 7;
		std::cout << c * (N % MOD) % MOD << '\n';
	}
}
