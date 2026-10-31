#include "Anna.h"
#include <vector>
#include <cmath>
#include <cassert>

namespace {

} // namespace

void Anna(int N, int K, std::vector<int> R, std::vector<int> C) {
	using namespace std;
	assert(K == 7);

	auto flag_at = [&](int r, int c) -> int {
		for (int k = 0; k < K; k++) {
			if (R[k] == r && C[k] == c) return k;
		}
		return K;
	};

	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			int idx = (r % 3) * 3 + (c % 3);
			int v;
			if (idx < K) {
				if (C[idx] - c > 1) {
					v = 0;
				} else if (C[idx] - c < -1) {
					v = 1;
				} else if (R[idx] - r > 1) {
					v = 2;
				} else if (R[idx] - r < -1) {
					v = 3;
				} else {
					int t_idx = (R[idx] % 3 * 3 + C[idx] % 3);
					v = 4 + t_idx;
				}
			} else if (idx == 7) {
				v = flag_at(r, c);
			} else if (idx == 8) {
				v = 13;
			} else assert(false);
			SetFlag(r, c, v+1);
		}
	}
}
