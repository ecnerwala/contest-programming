#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M, A, B; std::cin >> N >> M >> A >> B;
		std::vector<std::string> G(N);
		for (auto& r : G) {
			r.reserve(M);
			std::cin >> r;
			assert(int(r.size()) == M);
		}
	}

	return 0;
}
