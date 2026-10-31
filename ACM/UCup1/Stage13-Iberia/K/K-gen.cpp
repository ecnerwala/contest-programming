#include <bits/stdc++.h>

int main() {
	int N = int(1e5);
	int M = int(1e5);
	std::cout << N << ' ' << M << '\n';
	for (int i = 0; M > 0 && i < N; i++) {
		for (int j = 0; M > 0 && j < i; j++) {
			std::cout << i+1 << ' ' << j+1 << '\n';
			M--;
		}
	}
}
