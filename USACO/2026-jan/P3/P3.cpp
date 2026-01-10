#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int R, C, X; cin >> R >> C >> X;
		std::vector<std::pair<std::array<int, 2>, int>> constraints(X);
		for (auto& [p, v] : constraints) {
			char c; cin >> c >> p[0] >> p[1];
			assert(c == '+' || c == '-');
			v = c == '+' ? 0 : 1;
		}
		if (R < C) {
			std::swap(R, C);
			for (auto& [p, v] : constraints) std::swap(p[0], p[1]);
		}
		assert(C <= R);
		// monochromatic
		int64_t ans = 0;
		for (int av = 0; av < 2; av++) {
			bool good = true;
			for (auto [_, v] : constraints) {
				good = good && (v == av);
			}
			ans += good;
		}
		assert(R >= C);
		// There's monochromatic, 1 diagonal, and 2 diagonal
		// If R = 1 or C = 1, we can't distinguish the 2 types of diagonals from each other
		for (int diag_dir = 0; diag_dir < (C == 1 ? 1 : 2); diag_dir++) {
			if (diag_dir) {
				assert(C >= 2);
				// flip along the diagonal
				for (auto& [p, _] : constraints) {
					p[1] = C-1-p[1];
				}
			}

			for (int top_val = 0; top_val < 2; top_val++) {
				int bot_val = 1 ^ top_val;
				int max_top = 0;
				int min_bot = R+C-2;
				for (auto [p, v] : constraints) {
					int d = p[0] + p[1];
					int val = v ^ (d & 1);
					if (val == top_val) max_top = std::max(max_top, d);
					else if (val == bot_val) min_bot = std::min(min_bot, d);
					else assert(false);
				}
				// additionally, the split must be at max_top & 1 == top_val so that it's a +
				if ((max_top & 1) != top_val) max_top++;
				if ((min_bot & 1) != bot_val) min_bot--;
				if (max_top < min_bot) {
					ans += (min_bot - max_top + 1) / 2;
				}
			}

			for (int mid_val = 0; mid_val < 2; mid_val++) {
				int max_top = 0;
				int min_mid = C-1;
				int max_mid = R-1;
				int min_bot = R+C-2;
				for (auto [p, v] : constraints) {
					int d = p[0] + p[1];
					int val = v ^ (d & 1);
					if (val == mid_val) {
						min_mid = std::min(min_mid, d);
						max_mid = std::max(max_mid, d);
					} else {
						if (d <= R-1) {
							max_top = std::max(max_top, d);
						}
						if (d >= C-1) {
							min_bot = std::min(min_bot, d);
						}
					}
				}
				if ((max_top & 1) == mid_val) max_top++;
				if ((min_mid & 1) != mid_val) min_mid--;
				if ((max_mid & 1) != mid_val) max_mid++;
				if ((min_bot & 1) == mid_val) min_bot--;
				if (max_top < min_mid && max_mid < min_bot) {
					int extra_top = (min_mid - max_top - 1) / 2;
					int extra_bot = (min_bot - max_mid - 1) / 2;
					int cols = (max_top + 1) + ((R+C-1) - min_bot);
					int max_extra = C - cols;
					auto ways_extra = [&](int e) -> int64_t {
						if (e < 0) return 0;
						return int64_t(e+1) * int64_t(e+2) / 2;
					};
					extra_top++, extra_bot++;
					ans += ways_extra(max_extra) - ways_extra(max_extra - extra_top) - ways_extra(max_extra - extra_bot) + ways_extra(max_extra - extra_top - extra_bot);
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}

// We have a grid of 0/1, but 0->1 can only occur at particular parity.
//
// Another way of putting it is that any 2 neighboring must have at least a +.
// 0+ 1+
// 1+ 0+
// 1- 0-
//
// The 2x2 block of +'s only works if R = C = 2, otherwise it extends in at least 1 bad direction.
//
// In general, we get a forced "shockwave" when we have 0 and 1 touch
//    1+       0+ 0- 0+ 0- ...
// 1+ 1- 1+ 0+ 0- 0+ 0- 0+ ...
//    1+       0+ 0- 0+ ...
//
// Also, it has to extend up-left or down-right
