#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Lasso {

public:

	double lasso(int R, int C, int R1, int C1, int R2, int C2) {
		int D = std::max(R+1, C+1);
		std::vector<int> freq(D+1);
		for (int r = R1; r <= R2; r++) {
			for (int c = C1; c <= C2; c++) {
				int d = std::min(std::min(r - (-1), R + 1 - r), std::min(c - (-1), C + 1 - c));
				d = (d+1)/2;
				assert(1 <= d && d <= D);
				freq[d]++;
			}
		}
		std::vector<int> dists; dists.reserve((R2-R1+1) * (C2-C1+1));
		for (int d = 0; d <= D; d++) {
			for (int z = 0; z < freq[d]; z++) dists.push_back(d);
		}
		double ans = dists[0];
		for (int i = 0; i < int(dists.size()); i++) {
			ans = std::min(ans, double(dists[i]) / double(i+1));
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::Lasso;

// vim:ft=cpp
