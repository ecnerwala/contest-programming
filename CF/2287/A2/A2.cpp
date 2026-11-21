#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto eval = [&](int X, int Y, std::string S) -> int {
		std::string S; std::cin >> S;
		int ans = X + Y;
		int loc = 0;
		for (char c : S) {
			if (c == '+') {
				if (Y > 0) {
					Y++;
					ans = std::max(ans, X + Y);
				} else if (X > 0) {
					X--, Y++;
				}
			} else if (c == '-') {
				if (Y > 1) {
					Y--;
					X++;
				} else if (Y == 1) {
					if (X > 0) X--;
					else { X++, Y--; }
				} else {
					if (X > 0) X--;
				}
			} else assert(false);
		}
	};
	int T; std::cin >> T;
	while (T--) {
	}

	return 0;
}

// We should do sequences of +---, and then
