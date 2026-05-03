#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q; std::cin >> N >> Q;
		std::vector<int64_t> A(N); for (auto& a : A) std::cin >> a;

		int64_t base_X = 0;
		std::vector<int> x_costs(N+1);
		int64_t base_Y = 0;
		std::vector<int> y_costs(N+1);
		for (int st = 0, en = 0; st < N; st = en) {
			if (!A[st]) {
				en++;
				continue;
			}
			while (en < N && A[en] > 0) en++;
			// solve A[st..en]

			// first do the X >= Y case
			{
				std::vector<int> bad_pos; bad_pos.reserve(en - st);
				for (int i = st; i < en; i++) {
					assert(A[i] >= 1);
					base_X += A[i] / 2;
					if (A[i] & 1) {
						bad_pos.push_back(i);
					}
				}
				std::vector<int> bad_dist; bad_dist.reserve(bad_pos.size());
				for (int i = 0; i+1 < int(bad_pos.size()); i++) {
					int cost = bad_pos[i+1] - bad_pos[i];
					while (!bad_dist.empty() && cost >= bad_dist.back()) {
						x_costs[bad_dist.back()]++;
						cost -= bad_dist.back();
						bad_dist.pop_back();
						if (bad_dist.empty()) break;
						cost += bad_dist.back();
						bad_dist.pop_back();
					}
					bad_dist.push_back(cost);
				}
				while (!bad_dist.empty()) {
					x_costs[bad_dist.back()]++;
					bad_dist.pop_back();
					if (bad_dist.empty()) break;
					bad_dist.pop_back();
				}
			}

			// now, do the Y >= X case
			{
				// are all y_costs == 1 or 0??
				for (int i = 0; i < N; i++) {
				}
			}
		}

		std::vector<std::array<int64_t, 2>> x_vals(N+1);
		for (int i = 0; i <= N; i++) {
			std::cerr << i << ' '<< x_costs[i] << '\n';
			x_vals[i] = (i ? x_vals[i-1] : std::array<int64_t, 2>{base_X, 0});
			x_vals[i][0] += x_costs[i];
			x_vals[i][0] -= int64_t(x_costs[i]) * i;
			x_vals[i][1] += int64_t(x_costs[i]) * i;
		}

		for (int q = 0; q < Q; q++) {
			int x, y; std::cin >> x >> y;
			if (x >= y) {
				int max_cost = (x == y) ? N : std::min(N, x / (x - y));
				auto [cx, cy] = x_vals[max_cost];
				std::cout << (x * cx + y * cy) << '\n';
			} else {
				std::cout << "bad" << '\n';
			}
		}
	}

	return 0;
}
