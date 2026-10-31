#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);

	int T, M; std::cin >> T >> M;

	std::vector<int> sizes; sizes.reserve(T);
	std::vector<int> filled(M, false); filled.reserve(M);
	for (int t = 0; t < T; t++) {
		std::string op; std::cin >> op;
		if (op == "alloc") {
			int n; std::cin >> n;
			assert(n >= 1);
			bool found = false;
			for (int st = 0; st + n <= M; st++) {
				bool good = true;
				for (int i = 0; i < n; i++) {
					good = good && !filled[st+i];
				}
				if (good) {
					found = true;
					sizes.push_back(n);
					int x = int(sizes.size());
					std::cout << x << '\n';
					for (int i = 0; i < n; i++) {
						filled[st+i] = x;
					}
					break;
				}
			}
			if (!found) {
				std::cout << "NULL" << '\n';
			}
		} else if (op == "erase") {
			int64_t x; std::cin >> x;
			if (1 <= x && x <= int(sizes.size()) && sizes[x-1] > 0) {
				for (auto& v : filled) {
					if (v == x) {
						v = 0;
						sizes[x-1]--;
					}
				}
				sizes[x-1] = 0;
			} else {
				std::cout << "ILLEGAL_ERASE_ARGUMENT" << '\n';
			}
		} else if (op == "defragment") {
			int l = 0;
			for (int i = 0; i < M; i++) {
				if (filled[i] != 0) {
					filled[l] = filled[i];
					l++;
				}
			}
			for (;l < M; l++) filled[l] = 0;
		} else assert(false);
	}

	return 0;
}
