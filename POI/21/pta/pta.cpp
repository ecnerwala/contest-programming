#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
	int N; std::cin >> N;
	std::vector<int> D(N); for (auto& d : D) std::cin >> d;
	int Q; std::cin >> Q;

	std::array<std::vector<std::pair<int, int>>, 2> V;
	V[0].resize(N);
	V[1].resize(N);

	for (int q = 0; q < Q; q++) {
		int K; std::cin >> K;

		V[0][0] = {D[0], 0};
		int lo = 0, hi = 1, hi2 = 0;
		int res = 0;

		for (int i = 1; i < N; i++) {
			assert(lo < hi);
			bool z = D[i] >= V[0][lo].first;
			if (i == N-1) {
				std::cout << res+z << '\n';
				break;
			}
			if (z) {
				while (hi2 && D[i] >= V[1][hi2-1].first) {
					--hi2;
				}
				V[1][hi2++] = {D[i], i};
			} else {
				hi2 = 0;
				while (lo < hi && D[i] >= V[0][hi-1].first) {
					--hi;
				}
				V[0][hi++] = {D[i], i};
			}

			if (V[0][lo].second == i - K) {
				lo++;
				if (lo == hi) {
					lo = 0, hi = hi2;
					hi2 = 0;
					std::swap(V[0], V[1]);
					res++;
				}
			}
		}
	}
}
