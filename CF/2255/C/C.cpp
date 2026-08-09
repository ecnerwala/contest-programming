#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	std::string op; std::cin >> op;
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<std::string> G(N);
		for (auto& r : G) { r.reserve(N); std::cin >> r; assert(int(r.size()) == N); }

		int w = 0; 
		for (auto& r : G) for (auto& c : r) { w += c == '#'; }
		assert(std::gcd(w, N) == 1);
		assert(2 * w != N * N);
		if (w > N * N - w) {
			for (auto& r : G) for (auto& c : r) { c ^= '#' ^ '.'; }
			w = N*N-w;
		}

		int sr = 0;
		int sc = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (G[i][j] == '#') {
					sr += i;
					sc += j;
				}
			}
		}
		sr %= N;
		sc %= N;
		if (op == "first") {
			int r, c; std::cin >> r >> c; r--, c--;
			// we want the sum to be w * r % N
			r = r * w % N;
			c = c * w % N;
			int dr = (r - sr + N) % N;
			int dc = (c - sc + N) % N;
			// we want to increase the sum by dr / dc
			if (dr == 0 && dc == 0) {
				std::cout << 1 << ' ' << 1 << ' ' << 1 << ' ' << 1 << '\n';
			} else {
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < N; j++) {
						if (G[i][j] == '#' && G[(i+dr)%N][(j+dc)%N] == '.') {
							std::cout << i+1 << ' ' << j+1 << ' ' << (i+dr)%N+1 << ' ' << (j+dc)%N+1 << '\n';
							goto found;
						}
					}
				}
				assert(false);
found:
				;
			}
		} else if (op == "second") {
			int r = 0;
			while (r * w % N != sr) r++;
			int c = 0;
			while (c * w % N != sc) c++;
			std::cout << r+1 << ' ' << c+1 << '\n';
		} else assert(false);
	}

	return 0;
}
