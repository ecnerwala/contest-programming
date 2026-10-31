#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K, A, B; std::cin >> N >> K >> A >> B;
		std::cout << [&]() -> int {
			if (A > B) std::swap(A, B);
			assert(A <= B);
			int d = (B - A);
			if (d & 1) {
				assert(d >= 1);
				K -= std::min(A - 1, N - B) + 1;
				if (K < 0) {
					return 0;
				}
				d --;
			}
			K -= d / 2;
			if (K < 0) return 0;
			return (K + 1) / 2;
		}() << '\n';
	}

	return 0;
}
