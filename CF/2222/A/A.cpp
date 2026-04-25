#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		bool good = false;
		for (int i = 0; i < N; i++) {
			int a; std::cin >> a;
			if (a == 100) good = true;
		}
		std::cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
