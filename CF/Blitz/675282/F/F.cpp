#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		// only problem is 1's
		int L = 0;
		int R = N;
		while (L < R && A[L] == 1) L++;
		while (L < R && A[R-1] == 1) R--;
		// if the product is at least N^2, we just take everything
		__int128_t prod = 1;
		auto cap = __int128_t(N) * __int128_t(N);
		for (auto a : A) {
			prod *= a;
			prod = std::min(prod, cap);
		}
		if (L == R) {
			std::cout << 1 << ' ' << 1 << '\n';
		} else if (prod >= cap) {
			std::cout << L+1 << ' ' << R << '\n';
		} else {
			std::vector<int64_t> pref(N+1);
			for (int i = 0; i < N; i++) {
				pref[i+1] = pref[i] + A[i];
			}
			std::vector<int> bounds; bounds.reserve(30);
			for (int i = 0; i < N; i++) {
				if (A[i] > 1) bounds.push_back(i);
			}
			assert(!bounds.empty());
			using cnd_t = std::pair<int64_t, std::array<int, 2>>;
			cnd_t opt{0, {0, 0}};
			for (int i = 0; i < int(bounds.size()); i++) {
				int64_t p = 1;
				for (int j = i; j < int(bounds.size()); j++) {
					p *= A[bounds[j]];
					cnd_t cnd{p - (pref[j+1] - pref[i]), {i, j}};
					opt = std::max(opt, cnd);
				}
			}
			std::cout << opt.second[0]+1 << ' ' << opt.second[1]+1 << '\n';
		}
	}
}
