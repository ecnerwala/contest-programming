#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::array<int, 2> cnt{0, 0};
		for (int i = 0; i < N; i++) {
			int a; std::cin >> a;
			cnt[a & 1]++;
		}
		std::cout << ([&]() -> bool {
			assert(cnt[0] + cnt[1] >= 1);
			if (!(cnt[1] & 1)) {
				// Win immediately
				return true;
			}
			assert(cnt[1] & 1);

			if (cnt[0] + cnt[1] == 1) {
				return true;
			}

			// The spots that must get there are losing
			if (cnt[0] + cnt[1] / 2 == 1) {
				return false;
			}
			return true;
		}() ? "Moggity" : "Larpity") << '\n';
	}

	return 0;
}

// If there's at least one and the odd cnt is even, you just win.
// That means at the end of your turn, the odd cnt must be odd.
