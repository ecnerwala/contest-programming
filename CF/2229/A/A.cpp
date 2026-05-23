#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		int ma = *std::max_element(A.begin(), A.end());
		int mi = *std::min_element(A.begin(), A.end());
		std::cout << (ma - mi + 1) / 2 << '\n';
	}

	return 0;
}
