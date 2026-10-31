#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class MarriageAndCirclingChallenge {

public:

	long long solve(int N, int threshold, int state) {
		auto rnd = [&]() -> int {
			state = int((int64_t(state) * 1103515245ll + 12345ll) % (1ll << 31));
			return state % 100;
		};

		using bs = bitset<620>;
		vector<bs> loves(N);

		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if (rnd() < threshold) {
					loves[i][j] = true;
				} else {
					loves[j][i] = true;
				}
			}
		}
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				int a = int((loves[i] & ~loves[j]).count()) - loves[i][j];
				int b = int((loves[j] & ~loves[i]).count()) - loves[j][i];
				ans += a * b;
			}
		}
		ans /= 2;
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MarriageAndCirclingChallenge;

// vim:ft=cpp
