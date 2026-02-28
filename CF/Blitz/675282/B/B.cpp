#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto a : A) { std::cin >> a; }
		std::vector<int> B(N);
		[&](this auto&& self, int l, int r, int d) -> void {
			if (r - l == 0) return;
			int m = int(std::max_element(A.begin() + l, A.begin() + r) - A.begin());
			B[m] = d;
			self(l, m, d+1);
			self(m+1, r, d+1);
		}(0, N, 0);
		for (int i = 0; i < N; i++) std::cout << B[i] << " \n"[i+1==N];
	}
}
