#include <bits/stdc++.h>

int main() {
	int N = 10000;
	int M = 200000;
	std::cout << N << ' ' << M << '\n';
	for (int i = 0; i <= M; i++) std::cout << i % (N+1) << ' ';
	std::cout << '\n';
}
