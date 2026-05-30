#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		assert(M >= N);
		std::vector<std::pair<int, int>> known; known.reserve(N);
		for (int i = 0; i < N; i++) {
			int v; std::cin >> v;
			if (v == -1) continue;
			known.push_back({i, v});
		}
		auto res = [&]() -> std::tuple<int, int, int, int> {
			if (int(known.size()) == 0) {
				return {0, 1, 0, 1};
			} else if (int(known.size()) == 1) {
				int d = known[0].second - known[0].first;
				assert(d >= 0);
				assert(d <= M-N);
				return {0, 1, d, 1};
			}
			assert(int(known.size()) >= 2);
			auto check_xy = [&](int x, int y) -> std::optional<std::tuple<int, int, int, int>> {
				return {{-1, -1, -1, -1}};
			};
			// let's just check known[1] - known[0]
			for (int b = 1; b <= N-b; b++) {
				// we have b x's, and N-b y's
				int inner_x_lo = int(int64_t(b) * int64_t(known[1].first - known[0].first) / N);
				int inner_x_hi = int((int64_t(b) * int64_t(known[1].first - known[0].first) + N-1) / N);
				for (int64_t inner_x = inner_x_lo; inner_x <= inner_x_hi; inner_x++) {
					int64_t inner_y = known[1].first - known[0].first - inner_x;
					int64_t outer_x = b;
					int64_t outer_y = N-b;
					int64_t inner_tot = known[1].second - known[0].second;
					int64_t outer_tot = M;

					int64_t denom = inner_x * outer_y - outer_x * inner_y;
					assert(denom != 0);
					int64_t x_numer = inner_tot * outer_y - outer_tot * inner_y;
					int64_t y_numer = inner_x * outer_tot - outer_x * inner_tot;
					if (x_numer % denom != 0 || y_numer % denom != 0) continue;

					auto res = check_xy(int(x_numer / denom), int(y_numer / denom));
					if (res) return *res;
				}
			}
			assert(false);
		};
	}

	return 0;
}
