#include <bits/stdc++.h>

int main() {
	int N = 5000;
	int M = 15000;
	std::cout << N << ' ' << M << ' ' << 3 << '\n';
	for (int i = 0; i < N; i++) {
		std::cout << std::string(M/4, '0') << '\n';
	}
}
