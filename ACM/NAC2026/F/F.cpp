#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N; std::cin >> N;
	std::array<std::vector<std::pair<int, int>>, 2> locs;
	locs[0].reserve(N);
	locs[1].reserve(N);
	for (int i = 0; i < N; i++) {
		int x, y, v; std::cin >> x >> y >> v;
		locs[0].push_back({x+y, v});
		locs[1].push_back({x-y, v});
	}
	for (int z = 0; z < 2; z++) {
		std::sort(locs[z].begin(), locs[z].end());
	}

	auto is_good = [&](double t) -> bool {
		for (const auto& l : locs) {
			double lo = -1e18;
			double hi = 1e18;
			for (auto [p, v] : l) {
				lo = std::max(lo, p - v * t);
				hi = std::min(hi, p + v * t);
			}
			if (lo > hi) return false;
		}
		return true;
	};

	double mi = 0;
	double ma = 1e18;
	for (int iter = 0; iter < 300; iter++) {
		double md = mi + (ma - mi) / 2;
		if (is_good(md)) ma = md;
		else mi = md;
	}
	std::cout << std::fixed << std::setprecision(20) << mi << '\n';

	return 0;
}
