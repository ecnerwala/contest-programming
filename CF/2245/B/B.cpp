#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, C; std::cin >> N >> C;
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			int a; std::cin >> a;
			ans += std::max(0, a - C);
		}
		std::cout << ans << '\n';
	}

	return 0;
}
