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
			// first, check x == y, i.e. M % N == 0
			if (M % N == 0) {
				int64_t c = known[0].second - int64_t(known[0].first) * int64_t(M / N);
				bool good = c >= 0 && c < M/N;
				for (auto [i, j] : known) {
					if (c + i * int64_t(M/N) != j) good = false;
				}
				if (good) return {0, 1, c, M/N};
			}

			int global_min = std::min_element(known.begin(), known.end(), [&](auto a, auto b) -> bool {
				return int64_t(a.second) * N - int64_t(a.first) * M < int64_t(b.second) * N - int64_t(b.first) * M;
			})->first;

			auto check_xy = [&](int x, int y) -> std::optional<std::tuple<int, int, int, int>> {
				std::cerr << "check " << x << ' ' << y << '\n';
				assert(x != y);
				assert(x > 0 && y > 0);
				if (x > y) std::swap(x, y);
				assert(x < y);
				int b = (M - y * N) / (x - y);
				assert(x * b + y * (N-b) == M);
				for (auto st : {0, global_min, N-1}) {
					int64_t c = 0;
					auto eval_at = [&](int i) -> int64_t {
						int off = (i - st + N) % N;
						int64_t cnt_x = int64_t(b) * int64_t(off) / N;
						int64_t cnt_y = off - cnt_x;
						int64_t r = c + cnt_x * x + cnt_y * y;
						r %= M;
						return r;
					};
					c = (known[0].second - eval_at(known[0].first) + M) % M;
					// now, eval_at is a cycle which always does +x or +y, totalling to M

					bool good = eval_at(N-1) > eval_at(0);
					for (auto [i, j] : known) {
						if (eval_at(i) != j) good = false;
					}
					if (good) {
						return {{-1, -1, -1, -1}};
					}
				}
				return std::nullopt;
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

					if (x_numer == y_numer) continue;
					if (x_numer <= 0 || y_numer <= 0) continue;

					auto res = check_xy(int(x_numer / denom), int(y_numer / denom));
					if (res) return *res;
				}
			}
			assert(false);
		}();
		auto [a, b, c, d] = res;
		std::cout << a << ' ' << b << ' ' << c << ' ' << d << '\n';
	}

	return 0;
}
