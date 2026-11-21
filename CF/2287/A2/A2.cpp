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

	auto solve = [&](int N) -> std::string {
		std::string S; S.reserve(8 * N);
		for (int z = 0; z < 2 * N; z++) {
			S += "+---";
		}
		return S;
	};
	auto test = [&](int N) -> void {
		std::string S = solve(N);
		for (int X = 0; X <= N; X++) {
			int res = eval(X, N - X, S);
			std::cerr << X << ' ' << N-X << ' ' << res << '\n';
		}
		std::cerr << '\n';
	};
	for (int N = 1; N <= 10; N++) {
		test(N);
	}

	int T; std::cin >> T;
	while (T--) {
	}

	return 0;
}

// We should do sequences of +---, and then
