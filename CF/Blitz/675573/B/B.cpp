#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::string S; std::cin >> S;
		assert(int(S.size()) == N);
		// we can't remove leading 0's or trailing 1's
		int l = 0;
		while (l < N && S[l] == '0') l++;
		int r = N;
		while (r > 0 && S[r-1] == '1') r--;
		std::cout << std::string(l, '0');
		if (l < r) {
			std::cout << *std::min_element(S.begin() + l, S.begin() + r);
		}
		std::cout << std::string(N-r, '1');
		std::cout << '\n';
	}
}
