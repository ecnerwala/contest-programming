#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string S; std::cin >> S;
		assert(int(S.size()) == N);
		int64_t ans = 0;
		std::array<int, 3> cnt{0, 0, 0};
		int pref = 0;
		int cur_len = 0;
		for (int i = 0; i < N; i++) {
			pref += (S[i] - '0') + 1;
			pref %= 3;

			if (i == 0 || S[i] == S[i-1]) cur_len = 1;
			else cur_len++;

			ans += cnt[0]+cnt[1]+cnt[2] - pref - (cur_len - 1) / 2;

			cnt[pref]++;
		}
		std::cout << ans << '\n';
	}

	return 0;
}
