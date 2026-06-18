#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::vector<int> B(N); for (auto& b : B) std::cin >> b;
		for (int i = 0; i < N; i++) {
			A[i] = int(std::lower_bound(B.begin(), B.end(), A[i]) - B.begin());
		}
		// i -> >=A[i]
		std::cout << [&]() -> int {
			int ans = 0;
			for (int i = 0; i < N; i++) {
				int j;
				for (j = i; j < N; j++) {
					if (A[j] <= i) {
						break;
					}
				}
				if (j == N) return -1;
				for (; j > i; j--) { ans++, std::swap(A[j], A[j-1]); }
			}
			return ans;
		}() << '\n';
	}

	return 0;
}
