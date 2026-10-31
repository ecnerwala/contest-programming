#include <bits/stdc++.h>

int main() {
	using std::cin, std::cout;
	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		std::array<int, 4> freq{0, 0, 0, 0};
		for (char c : S) {
			if (c == 'L') freq[0]++;
			else if (c == 'R') freq[1]++;
			else if (c == 'U') freq[2]++;
			else if (c == 'D') freq[3]++;
			else assert(false);
		}
		int lr = std::min(freq[0], freq[1]);
		int ud = std::min(freq[2], freq[3]);
		if (lr > 0 && ud > 0) {
			std::cout << 2 * (lr + ud) << '\n';
			for (int i = 0; i < lr; i++)  std::cout << 'R';
			for (int i = 0; i < ud; i++)  std::cout << 'U';
			for (int i = 0; i < lr; i++)  std::cout << 'L';
			for (int i = 0; i < ud; i++)  std::cout << 'D';
			std::cout << '\n';
		} else if (lr > 0) {
			std::cout << 2 << '\n' << "RL" << '\n';
		} else if (ud > 0) {
			std::cout << 2 << '\n' << "UD" << '\n';
		} else {
			std::cout << 0 << '\n' << '\n';
		}
	}
}
