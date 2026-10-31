#include <bits/stdc++.h>

int main() {
	int Q = 200;
	std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	std::vector<int> sizes;
	sizes.push_back(0);
	std::cout << 2 * Q << '\n';
	for (int i = 0; i < Q; i++) {
		while (true) {
			int op = std::uniform_int_distribution<int>(1, 10)(rng);
			if (op <= 5) {
				int a = std::uniform_int_distribution<int>(1, 10)(rng);
				std::cout << '+' << ' ' << a << '\n';
				sizes.push_back(sizes.back() + 1);
				break;
			} else if (op <= 8) {
				if (sizes.back() == 0) continue;
				int k = std::uniform_int_distribution<int>(1, sizes.back())(rng);
				std::cout << '-' << ' ' << k << '\n';
				sizes.push_back(sizes.back() - k);
				break;
			} else {
				if (sizes.size() == 1) continue;
				std::cout << '!' << '\n';
				sizes.pop_back();
				break;
			}
		}
		std::cout << '?' << '\n';
	}
}
