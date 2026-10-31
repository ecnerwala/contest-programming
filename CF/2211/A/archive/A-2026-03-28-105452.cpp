#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		for (int i = 0; i < N; i++) { int a; std::cin >> a; }
		for (int i = 0; i < N; i++) {
			std::cout << (N == 1 ? 1 : 2) << " \n"[i+1==N];
		}
	}

	return 0;
}
