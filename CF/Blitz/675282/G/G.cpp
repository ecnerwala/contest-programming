#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N, M, K; std::cin >> N >> M >> K;
		std::vector<int> cnt(M);
		for (int k = 0; k < K; k++) {
			int x, y; std::cin >> x >> y; x--, y--;
			assert(0 <= y && y < M);
			if (y == 0) continue;
			if (N >= 2) {
				cnt[y] ^= 1;
			} else {
				if (y == 1) cnt[y] ^= 1;
			}
		}
		bool first = false;
		for (auto v : cnt) if (v) first = true;
		std::cout << (first ? "Mimo" : "Yuyu") << '\n';
	}
}
