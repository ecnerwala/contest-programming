#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { std::cin >> a; a--; }
		std::vector<int> B(N);
		for (auto& b : B) { std::cin >> b; if (b != -1) b--; }

		if ([&]() -> bool {
			for (int st = 0; st < N; st++) {
				if (B[st] == -1) continue;
				for (int i = st; true; ) {
					if (B[A[i]] == -1) {
						B[A[i]] = A[B[i]];
						continue;
					} else {
						if (B[A[i]] != A[B[i]]) return false;
						else break;
					}
					assert(false);
				}
			}
			return true;
		}()) {
			std::cout << "YES" << '\n';
			for (int i = 0; i < N; i++) std::cout << B[i]+1 << " \n"[i+1==N];
		} else {
			std::cout << "NO" << '\n';
		}
	}

	return 0;
}
