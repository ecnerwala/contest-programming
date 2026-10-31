#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N, M, K; std::cin >> N >> M >> K;
		std::vector<int> cnt(M);
		for (int k = 0; k < K; k++) {
			int x, y; std::cin >> x >> y;
			x--, y--;
			if (N >= 2) {
				cnt[y]++;
			} else {
				if (!(y & 1)) cnt[0] ++;
			}
		}
		bool first = false;
		for (int i = 0; i < M; i++) {
			if (cnt[i] & 1) first = true;
		}
		std::cout << (first ? "Mimo" : "Yuyu") << '\n';
	}
}
