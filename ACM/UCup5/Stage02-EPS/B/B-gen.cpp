#include <bits/stdc++.h>

int main() {
	std::mt19937 mt(48);
	int N = int(1e5);
	int Q = int(1e5);
	std::cout << N << ' ' << Q << '\n';
	std::vector<int> A(N);
	for (int i = 0; i < N; i++) {
		A[i] = std::uniform_int_distribution<int>(1, int(1e5))(mt);
	}
	for (int i = 0; i < N; i++) {
		std::cout << A[i] << " \n"[i+1==N];
	}
	for (int q = 0; q < Q; q++) {
		int l = std::uniform_int_distribution<int>(1, N)(mt);
		int r = std::uniform_int_distribution<int>(1, N)(mt);
		if (l > r) std::swap(l, r);
		int k = std::uniform_int_distribution<int>(0, (r-l+1)/2)(mt);
		std::cout << l << ' ' << r << ' ' << k << '\n';
	}
}
