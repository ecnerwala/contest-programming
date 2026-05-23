#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<std::array<int, 2>> A(N);
		for (int z = 0; z < 2; z++) for (auto& a : A) std::cin >> a[z];

		auto is_good = [&](int v) -> bool {
			std::cerr << "testing " << v << '\n';
			bool was_neg = false;
			int tot_val = 0;
			for (auto a : A) {
				int s = (a[0] >= v) + (a[1] >= v) - 1;
				std::cerr << "s = " << s << '\n';
				if (s == 0) continue;
				else if (s == -1) {
					if (was_neg) continue;
					was_neg = true;
					tot_val--;
				} else if (s == +1) {
					tot_val++;
					was_neg = false;
				} else assert(false);
			}
			std::cerr << v << ' ' << tot_val << '\n';
			return tot_val > 0;
		};

		int mi = 1;
		int ma = 2*N;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (is_good(md)) {
				mi = md;
			} else {
				ma = md;
			}
		}
		std::cout << mi << '\n';
	}

	return 0;
}

// just consider the 0-1 problem, and we only care about s[i] = a[i] + b[i] - 1
// s[i] + s[i+1] == -2 => -1
// s[i] + s[i+1] == -1 => -1
// s[i] + s[i+1] == 0 => 0
// s[i] + s[i+1] == +1 => +1
// s[i] + s[i+1] == +2 => +1
// We want to end at +1.
// 0's just don't do anything when merged anywhere, so we should delete them.
// + and - can cancel
// We want to group together -'s, then we win if there's at least as many +'s as -'s
