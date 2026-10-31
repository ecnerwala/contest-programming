#include <vector>
#include <array>
#include <cassert>

void riprogramma(int N, int K, std::vector<int>& C) {
	if (K >= 3) {
		for (int i = 1; i < N; i++) {
			if (C[i-1] == C[i]) {
				C[i] = 1;
				while (C[i] == C[i-1] || (i+1 < N && C[i] == C[i+1])) C[i]++;
			}
		}
	} else if (K == 2) {
		std::array<int, 2> cost{0,0};
		for (int i = 0; i < N; i++) {
			cost[!(((C[i] - 1) ^ i) & 1)]++;
		}
		int v = cost[0] < cost[1] ? 0 : 1;
		for (int i = 0; i < N; i++) {
			C[i] = ((i ^ v) & 1) + 1;
		}
	} else assert(false);
}
