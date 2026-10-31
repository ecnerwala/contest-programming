#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N; std::cin >> N;
	if (N == 3) {
		std::cout << R"(111
111
001

011
111
100

110
110
110

010
111
111

111
111
111

010
111
101

111
111
110

111
011
111

011
010
111

DURLDULRUU)" << '\n';
		return 0;
	}
	assert(N >= 4);
	for (int a = 0; a < N; a++) {
		for (int b = 0; b < N; b++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (i == a) {
						if (j == b-1) {
							std::cout << '1';
						} else {
							std::cout << '0';
						}
					} else if (i == a-1) {
						if (j == 0 || j == 1) std::cout << '0';
						else std::cout << '1';
					} else {
						std::cout << '1';
					}
				}
				std::cout << '\n';
			}
			std::cout << '\n';
		}
	}
	for (int i = 0; i <= N*N; i++) {
		if (i % N == (N-2) || i % N == (N-1)) {
			std::cout << 'U';
		} else if (i == 0) {
			std::cout << 'R';
		} else if (i == 1) {
			std::cout << 'L';
		} else if (i % N == 0 || i % N == 1) {
			std::cout << 'D';
		} else {
			std::cout << 'U';
		}
	}
	std::cout << '\n';

	return 0;
}
