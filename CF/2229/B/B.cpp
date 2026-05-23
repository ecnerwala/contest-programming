#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::vector<int> B(N); for (auto& b : B) std::cin >> b;
		for (int i = 0; i < N; i++) {
			if (A[i] > B[i]) std::swap(A[i], B[i]);
		}
		std::cout << std::accumulate(B.begin(), B.end(), int64_t(*std::max_element(A.begin(), A.end()))) << '\n';
	}

	return 0;
}
