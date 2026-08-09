#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::string S; S.reserve(2*N); std::cin >> S;
		std::array<int, 2> ans{0, 0};
		for (int i = 0; i < 2*N; i++) {
			if (S[i] == '1' && S[(i+1)%(2*N)] == '1') {
				// we're bad
				ans[i&1]++;
			} else if (S[i] == '0' && S[(i+2*N-1)%(2*N)] == '1') {
				ans[i&1]++;
			}
		}
		std::cout << ans[1] << ' ' << ans[0] << '\n';
	}

	return 0;
}
