#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::vector<int> B(N); for (auto& b : B) std::cin >> b;

		std::cout << ([&]() -> bool {
			// for each mod K, if it's periodic
			for (int m = 0; m < K; m++) {
				bool is_periodic = true;
				for (int i = m; i+K < N; i += K) {
					if (A[i] != A[i+K]) is_periodic = false;
				}
				if (!is_periodic) {
					assert(m+K < N);
					// they must all match
					for (int i = m; i < N; i += K) {
						if (B[i] == -1) B[i] = A[i];
						if (A[i] != B[i]) return false;
					}
				} else {
					// they must be all equal
					int b = -1;
					for (int i = m; i < N; i += K) {
						if (B[i] != -1) { b = B[i]; break; }
					}
					if (b != -1) {
						for (int i = m; i < N; i += K) {
							if (B[i] == -1) { B[i] = b; }
							if (B[i] != b) return false;
						}
					}
					// now, it's either -1 or not -1
				}
			}
			std::vector<int> cnt(N+1);
			for (int i = 0; i < K; i++) {
				cnt[A[i]]++;
				if (B[i] != -1) cnt[B[i]]--;
			}
			for (auto v : cnt) if (v < 0) return false;
			return true;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
