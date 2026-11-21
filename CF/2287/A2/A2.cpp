#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto eval = [&](int X, int Y, std::string S) -> int {
		int ans = X + Y;
		int loc = 0;
		for (int i = 0; i < int(S.size()); i++) {
			char c = S[i];
			if (c == '+') {
				if (Y > 0) {
					Y++;
					if (X + Y > ans) { ans = X + Y, loc = i+1; }
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
		return loc;
	};

	auto test = [&](int N, std::string S) -> void {
	}

	int T; std::cin >> T;
	while (T--) {
	}

	return 0;
}

// We should do sequences of +---, and then
