#include <bits/stdc++.h>

int main() {
	int N = 5;
	int S = 4;
	int Q = 10000;
	std::mt19937 mt(48);
	std::cout << N << ' ' << S << ' ' << Q << '\n';
	std::vector<int> P(N); std::iota(P.begin(), P.end(), 0);
	std::shuffle(P.begin(), P.end(), mt);
	for (int i = 0; i < N; i++) std::cout << P[i]+1 << '\n';
	for (int q = 0; q < Q; q++) {
		int k = mt() % N; int a = mt() % 70;
		std::cout << k+1 << ' ' << a+1 << '\n';
	}
}
