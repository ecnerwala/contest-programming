#include <bits/stdc++.h>

int main() {
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) { std::cin >> a; a--; }
		int num_bad = 0;
		for (auto a : A) num_bad += (a == -1);
		if (num_bad == 1) {
			int sum = 0;
			for (int i = 0; i < N; i++) {
				sum ^= i;
				if (A[i] != -1) sum ^= A[i];
			}
			for (int i = 0; i < N; i++) {
				if (A[i] == -1) A[i] = sum;
			}
		}
		int l = 0, r = N;
		while (l < r && A[l] == l) l++;
		while (l < r && A[r-1] == r-1) r--;
		std::cout << r-l << '\n';
	}
}
