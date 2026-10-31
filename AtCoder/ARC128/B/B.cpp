#include <bits/stdc++.h>

#define ecnerlawa main

int ecnerlawa() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::array<int, 3> V; cin >> V[0] >> V[1] >> V[2];
		std::sort(V.begin(), V.end());

		int ans = -1;
		do {
			auto [A, B, C] = V;
			int b = 2 * B + C;
			if (b % 3) continue;
			b /= 3;
			int a = B - b;
			if (a < 0) continue;
			assert(b - 2 * a == C);

			if (A == 0 && C == 0) {
				continue;
			}
			if (ans == -1 || a+b < ans) ans = a+b;
		} while (std::next_permutation(V.begin(), V.end()));
		cout << ans << '\n';
	}

	return 0;
}

/*
 * a * [-1, -1, 2]
 * b * [2, -1, -1]
 * Let's say we make A's, then a + b = B and b - 2a = C
 * (B + 2C)
 */
