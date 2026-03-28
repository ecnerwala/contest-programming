#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::vector<int> B(N); for (auto& b : B) std::cin >> b;

		std::cout << ([&]() -> bool {
			// the first N-K must be equal, the last N-K must be equal
			for (int i = 0; i < N-K; i++) {
				if (B[i] != -1 && A[i] != B[i]) return false;
			}
			for (int i = N-1; i >= K; i--) {
				if (B[i] != -1 && A[i] != B[i]) return false;
			}
			if (N-K <= K) {
				std::vector<int> cnt(N+1);
				for (int i = N-K; i < K; i++) {
					cnt[A[i]]++;
					if (B[i] != -1) cnt[B[i]]--;
				}
				for (auto v : cnt) if (v < 0) return false;
			}
			return true;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
