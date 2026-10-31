#include <bits/stdc++.h>

int main() {
	std::vector<int> pool;
	pool.push_back(1);
	for (int i = 2; i < (1 << 30); i *= 2) {
		pool.push_back(i);
		pool.push_back(i + i/2);
	}
	int N = 3e5;
	std::cout << N << '\n';
	for (int i = 0; i < N; i++) {
		std::cout << pool[i % int(pool.size())] << " \n"[i+1==N];
	}
}
