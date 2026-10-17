#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		assert(M == 0);
		if (N == 1) {
			std::cout << 0 << '\n';
		} else {
			std::cout << (N+1)/2 << '\n';
			for (int i = 0; i < (N+1)/2; i++) {
				int l = i, r = i + (N+1)/2;
				std::cout << l+1 << ' ' << r << '\n';
			}
		}
	}

	return 0;
}
