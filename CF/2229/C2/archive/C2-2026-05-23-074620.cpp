#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;

		int64_t best_delta = 0;
		int best_idx = -1;

		int64_t cur_delta = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] < 0) {
			} else {
				cur_delta -= 2 * A[i];
				if (cur_delta > best_delta) {
					best_delta = cur_delta;
					best_idx = i;
				}
			}
			cur_delta += 2 * A[i];
		}

		std::vector<int> ops; ops.reserve(N);
		if (best_idx >= 0) {
			bool flip = false;
			for (int i = best_idx-1; i >= 0; i--) {
				if ((A[i] > 0) != flip) {
					ops.push_back(i);
					flip = !flip;
				}
			}
			ops.push_back(best_idx);
		}

		std::cout << ops.size() << '\n';
		for (int i = 0; i < int(ops.size()); i++) {
			if (i) std::cout << ' ';
			std::cout << ops[i]+1;
		}
		std::cout << '\n';
	}

	return 0;
}
