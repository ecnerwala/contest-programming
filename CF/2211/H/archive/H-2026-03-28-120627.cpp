#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		int64_t tot = 0;
		int64_t w_tot = 0;
		for (int i = 0; i < N; i++) {
			tot += A[i];
			w_tot += i * int64_t(A[i]);
		}
		if (w_tot * 2 >= (N-1) * tot) {
			std::cout << "Yes" << '\n';
		} else {
			std::cout << "No" << '\n';
		}
	}

	return 0;
}
