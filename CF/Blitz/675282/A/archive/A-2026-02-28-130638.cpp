#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) { std::cin >> a; a--; }
		int l = 0, r = N;
		while (l < r && A[l] == l) l++;
		while (l < r && A[r-1] == r-1) r--;
		if (r - l == 1) {
			std::cout << 0 << '\n';
		} else {
			std::cout << r-l << '\n';
		}
	}
}
