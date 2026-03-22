#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	std::string S; std::cin >> S;
	int M; std::cin >> M;
	int cnt_A = 0;
	int64_t ans = 0;
	for (char c : S) {
		if (c == 'A') {
			cnt_A++;
		} else if (c == 'C' || c == 'K') {
			ans += cnt_A;
		} else assert(false);
	}
	std::cout << std::max(ans - M, int64_t(0)) << '\n';

	return 0;
}
