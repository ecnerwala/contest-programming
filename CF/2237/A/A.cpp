#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N);
		for (int i = 0; i < N; i++) {
			std::cin >> A[i];
			if (i) A[i] = std::min(A[i], A[i-1]);
		}
		std::cout << std::accumulate(A.begin(), A.end(), 0) << '\n';
	}

	return 0;
}
