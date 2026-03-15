#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q; std::cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		std::array<std::set<std::pair<int, int>>, 2> vals;
		std::array<int64_t, 2> sums{0, 0};
		int cutoff = (N+1)/2;
		for (int i = 0; i < N; i++) {
			sums[i >= cutoff] += A[i];
			vals[i >= cutoff].insert({A[i], i});
		}

		auto print_ans = [&]() -> void {
			assert(!vals[0].empty());
			assert(!vals[1].empty());
			std::array<int, 2> ends{vals[0].begin()->first, vals[1].begin()->first};
			std::array<int64_t, 2> res = sums;
			res[0] -= ends[0];
			res[1] -= ends[1];
			if (N & 1) {
				std::sort(ends.begin(), ends.end());
			} else {
				std::sort(ends.begin(), ends.end(), std::greater<>());
			}
			res[0] += ends[0];
			res[1] += ends[1];
			std::cout << res[0] << ' ' << res[1] << '\n';
		};

		print_ans();

		for (int q = 0; q < Q; q++) {
			int i, a; std::cin >> i >> a; i--;
			sums[i >= cutoff] -= A[i];
			vals[i >= cutoff].erase({A[i], i});

			A[i] = a;

			sums[i >= cutoff] += A[i];
			vals[i >= cutoff].insert({A[i], i});

			print_ans();
		}
	}

	return 0;
}

// each person holds a carrot, and then they take turns taking from the middle
// each person eats their entire half except
