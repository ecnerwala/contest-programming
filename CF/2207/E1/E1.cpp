#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		bool good = true;
		for (int i = 0; i+1 < N; i++) {
			if (A[i] < A[i+1]) {
				good = false;
			}
		}
		for (int i = 0; i < N; i++) {
			if (A[i] < N-i-1) {
				good = false;
			}
		}
		for (int i = 0; i < N; i++) {
			if (A[i] > N) {
				good = false;
			}
		}
		if (good) {
			// start with 0..N+1
			std::vector<int> B; B.reserve(N+1);
			std::vector<int> stk; stk.reserve(N+1);
			int prv = -1;
			for (int i = N-1; i >= 0; i--) {
				if (A[i] > prv) {
					while (prv < A[i]) { prv++; stk.push_back(prv); }
					assert(stk.back() == A[i]);
				}
				assert(!stk.empty());
				B.push_back(stk.back());
				stk.pop_back();
			}
			std::reverse(B.begin(), B.end());
			B.pop_back();
			std::cout << "YES" << '\n';
			for (int i = 0; i < N; i++) std::cout << B[i] << " \n"[i+1==N];
		} else {
			std::cout << "NO" << '\n';
		}
	}

	return 0;
}
