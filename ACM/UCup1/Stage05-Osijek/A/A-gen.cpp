#include <bits/stdc++.h>

int main() {
	int N = int(1e5);
	std::cout << N << '\n';
	for (int i = 0; i < N; i++) {
		std::cout << ((1 << 25) - 1) << " \n"[i+1==N];
	}
	for (int i = 0; i < N-1; i++) {
		int j = i+1;
		std::cout << i+1 << ' ' << j+1 << '\n';
	}
}
