#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q; std::cin >> N >> Q;
		std::vector<int> A(N, 0);
		assert(N >= 2);
		int cnt_good = N;
		auto set_val = [&](int i, int v) -> void {
			cnt_good -= !(__builtin_popcount(A[i]) & 1);
			A[i] = v;
			cnt_good += !(__builtin_popcount(A[i]) & 1);
		};
		for (int i = 0; i < N; i++) {
			int a; std::cin >> a;
			set_val(i, a);
		}
		std::cout << cnt_good;
		for (int q = 0; q < Q; q++) {
			int p, x; std::cin >> p >> x; p--;
			set_val(p, x);
			std::cout << ' ' << cnt_good;
		}
		std::cout << '\n';
	}

	return 0;
}
