#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::string S; S.reserve(N); std::cin >> S;
		if (N < 2 * K) std::cout << -1 << '\n';
		else {
			int ans = 0;
			for (int i = 0; i < K; i++) {
				ans += S[i] != 'R';
				ans += S[N-1-i] != 'L';
			}
			std::cout << ans << '\n';
		}
	}

	return 0;
}
