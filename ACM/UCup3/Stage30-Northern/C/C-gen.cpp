#include <bits/stdc++.h>

int main() {
	int N; std::cin >> N;
	int r = 1;
	for (int z = 0; z < N; z++) r *= 6;
	std::cout << N << '\n';
	std::cout << std::string(r, '1') << '\n';
}
