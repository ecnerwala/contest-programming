#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int X, Y, M; std::cin >> X >> Y >> M;
		std::string S; std::cin >> S;
		int ans = X + Y;
		for (char c : S) {
			if (c == '+') {
				if (Y > 0) {
					Y++;
				} else if (X > 0) {
					X--, Y++;
				}
			} else if (c == '-') {
			}
		}
	}

	return 0;
}
