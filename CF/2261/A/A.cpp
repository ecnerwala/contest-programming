#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string S; S.reserve(N); std::cin >> S;
		assert(int(S.size()) == N);
		int cnt = 0;
		int mi = N, ma = -1;
		for (int i = 0; i < N; i++) {
			if (S[i] == '1') {
				cnt++;
				mi = std::min(mi, i);
				ma = std::max(ma, i);
			}
		}
		assert(cnt >= 1);
		if (cnt == 1) {
			std::cout << 0 << '\n';
		} else if (cnt == 2) {
			std::cout << 1 << '\n';
		} else if (cnt >= 3) {
			std::cout << ma - mi << '\n';
		}
	}
	return 0;
}

// If we have at least 3 numbers, it's just our original range
// If we have 1 nu ber
