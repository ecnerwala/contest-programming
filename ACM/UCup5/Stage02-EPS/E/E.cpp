#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::array<std::vector<int>, 3> R;
		std::array<std::vector<int>, 3> invR;
		for (int z = 0; z < 3; z++) {
			R[z].resize(N);
			invR[z].resize(N);
			for (int i = 0; i < N; i++) {
				int v; std::cin >> v; v--;
				R[z][i] = v;
				invR[z][v] = i;
			}
		}
		std::vector<bool> good(N-1);
		for (int z = 0; z < 3; z++) {
			for (int y = z+1; y < 3; y++) {
				int j = 0;
				for (int i = 0; i < N-1; i++) {
					j = std::max(invR[y][R[z][i]], j);
					assert(j >= i);
					if (j == i) {
						good[i] = true;
					}
				}
			}
		}
		std::cout << std::accumulate(good.begin(), good.end(), 1) << '\n';
	}

	return 0;
}
