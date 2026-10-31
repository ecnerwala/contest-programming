#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int X, Y, Z, A, B, C; std::cin >> X >> Y >> Z >> A >> B >> C;
		int ans = (X + Y + Z) * B;
		for (int num_mixed = 0; num_mixed <= X + Y + Z; num_mixed++) {
			int rem_x = std::max(X - num_mixed, 0), rem_y = std::max(Y - num_mixed, 0);
			int rem_z = std::max(Z - std::max(num_mixed - X, 0) - std::max(num_mixed - Y, 0), 0);
			int cost = num_mixed * B;
			cost += A * ((rem_x + 1) / 2);
			if ((rem_x & 1) && rem_z) rem_z--;
			cost += C * ((rem_y + 1) / 2);
			if ((rem_y & 1) && rem_z) rem_z--;
			cost += std::min(A, C) * ((rem_z + 1) / 2);
			ans = std::min(ans, cost);
		}
		std::cout << ans << '\n';
	}

	return 0;
}
