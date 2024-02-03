#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::tuple<int, int, int>> pts(N);
	std::vector<int> Ys; Ys.reserve(N);
	for (auto& [x, y, w] : pts) {
		cin >> x >> y >> w;
		Ys.push_back(y);
	}
	std::sort(pts.begin(), pts.end(), [&](auto a, auto b) { return std::get<0>(a) < std::get<0>(b); });

	std::sort(Ys.begin(), Ys.end());
	Ys.resize(std::unique(Ys.begin(), Ys.end()) - Ys.begin());

	int64_t ans = 0;
	for (int j0 = 0; j0 < int(Ys.size()); j0++) {
		for (int j1 = j0; j1 < int(Ys.size()); j1++) {
			auto ylo = Ys[j0];
			auto yhi = Ys[j1];
			int64_t best_region = 0;
			int prv_x = 0;
			for (auto [x, y, w] : pts) {
				if (y < ylo || yhi < y) continue;
				best_region = std::max(best_region - 2 * (x - prv_x), int64_t(0));
				prv_x = x;
				best_region += w;
			}
			ans = std::max(ans, best_region - 2 * (yhi - ylo));
		}
	}
	cout << ans << '\n';

	return 0;
}
