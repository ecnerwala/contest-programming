#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::vector<int> ops; ops.reserve(N);
		bool flip = false;
		for (int i = N-1; i >= 0; i--) {
			if ((A[i] < 0) != flip) {
				ops.push_back(i);
				flip = !flip;
			}
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
