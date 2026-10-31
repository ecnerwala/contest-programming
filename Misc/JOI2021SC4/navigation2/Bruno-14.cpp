#include "Bruno.h"
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

namespace {

} // namespace

std::vector<int> Bruno(int K, std::vector<int> value) {
	assert(K == 7);

	for (auto& v : value) v--;

	int r0 = -1;
	int c0 = -1;
	for (int i = 0; i < 9; i++) {
		if (value[i] == 13) {
			assert(r0 == -1 && c0 == -1);
			r0 = (i / 3 + 1) % 3;
			c0 = (i % 3 + 1) % 3;
		}
	}

	//std::cerr << r0 << ' ' << c0 << '\n';
	assert(r0 != -1 && c0 != -1);

	int cell_7 = value[(r0+2)%3*3+(c0+1)%3];

	std::vector<int> ans(K);

	for (int k = 0; k < K; k++) {
		int r = (r0 + k / 3) % 3;
		int c = (c0 + k % 3) % 3;
		int v = value[r*3+c];
		//std::cerr << "flag " << k << ' ' << v << '\n';
		if (v < 4) {
			ans[k] = v;
		} else {
			v -= 4;
			/*
			if (v == 7) {
				v = (cell_7 == k ? 7 : 8);
			}
			*/

			//std::cerr << "target " << v/3 << ' ' << v%3 << '\n';
			int r_tgt = (v / 3 + r0 + 3 - (r-1)) % 3 + r-1;
			int c_tgt = (v % 3 + c0 + 3 - (c-1)) % 3 + c-1;
			//std::cerr << "target " << r_tgt << ' ' << c_tgt << '\n';
			if (r_tgt > 1) {
				ans[k] = 2;
			} else if (r_tgt < 1) {
				ans[k] = 3;
			} else if (c_tgt > 1) {
				ans[k] = 0;
			} else if (c_tgt < 1) {
				ans[k] = 1;
			} else {
				ans[k] = 4;
			}
		}
	}

	return ans;
}
