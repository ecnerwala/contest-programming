#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<std::pair<int, int>> A(N);
		for (auto& [a, i] : A) std::cin >> a;
		for (int i = 0; i < N; i++) A[i].second = i;
		std::sort(A.begin(), A.end());

		std::vector<std::array<std::array<int, 2>, 2>> ranges, nranges0, nranges1;
		ranges.reserve(N); nranges0.reserve(N); nranges1.reserve(N);
		assert(N >= 2);
		ranges.push_back({std::array<int, 2>{0, N}, {0, N}});
		for (int b = K-1; b >= 0; b--) {
			nranges1.clear();
			nranges0.clear();
			for (auto [rng1, rng2] : ranges) {
				auto [l1, r1] = rng1;
				int m1 = l1;
				while (m1 < r1 && !((A[m1].first >> b) & 1)) m1++;
				if (rng1 == rng2) {
					if (m1 - l1 >= 2) {
						nranges1.push_back({std::array<int, 2>{l1, m1}, {l1, m1}});
					}
					if (r1 - m1 >= 2) {
						nranges1.push_back({std::array<int, 2>{m1, r1}, {m1, r1}});
					}
					if (m1 - l1 >= 1 && r1 - m1 >= 1) {
						nranges0.push_back({std::array<int, 2>{l1, m1}, {m1, r1}});
					}
				} else {
					auto [l2, r2] = rng2;
					assert(r1 <= l2);
					int m2 = l2;
					while (m2 < r2 && !((A[m2].first >> b) & 1)) m2++;
					assert(l2 <= m2 && m2 <= r2);
					if (m1 - l1 >= 1 && m2 - l2 >= 1) {
						nranges1.push_back({std::array<int, 2>{l1, m1}, {l2, m2}});
					}
					if (r1 - m1 >= 1 && r2 - m2 >= 1) {
						nranges1.push_back({std::array<int, 2>{m1, r1}, {m2, r2}});
					}
					if (m1 - l1 >= 1 && r2 - m2 >= 1) {
						nranges0.push_back({std::array<int, 2>{l1, m1}, {m2, r2}});
					}
					if (r1 - m1 >= 1 && m2 - l2 >= 1) {
						nranges0.push_back({std::array<int, 2>{m1, r1}, {l2, m2}});
					}
				}
			}
			if (!nranges1.empty()) {
				std::swap(ranges, nranges1);
			} else {
				std::swap(ranges, nranges0);
			}
		}
		assert(!ranges.empty());
		auto [rng0, rng1] = ranges[0];
		int i = rng0[0], j = rng1[1] - 1;
		assert(i < j);
		int x = ((1 << K) - 1) & ~A[i].first & ~A[j].first;
		std::cout << A[i].second+1 << ' ' << A[j].second+1 << ' ' << x << '\n';
	}

	return 0;
}
