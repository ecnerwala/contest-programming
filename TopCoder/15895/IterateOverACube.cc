#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class IterateOverACube {

public:

	vector <int> findCell(int _N, ll index) {
		ll N = _N;

		for (ll sum = 0; sum <= 3 * N - 3; sum++) {
			ll sumCnt;
			if (sum <= N-1) {
				sumCnt = (sum+1) * (sum+2) / 2;
			} else if (sum >= 2 * (N-1)) {
				ll negSum = 3 * (N-1) - sum;
				sumCnt = (negSum+1) * (negSum + 2) / 2;
			} else {
				assert(N-1 < sum && sum < 2 * (N-1));
				sumCnt = (sum+1) * (sum+2) / 2 - 3 * (sum - N + 1) * (sum - N + 2) / 2;
			}
			if (index >= sumCnt) {
				index -= sumCnt;
				continue;
			}

			for (ll x = 0; x <= N-1; x++) {
				ll remSum = sum - x;
				ll xCnt;
				if (0 <= remSum && remSum <= 2*(N-1)) {
					xCnt = min(remSum, 2*(N-1) - remSum) + 1;
				} else {
					xCnt = 0;
				}
				if (index >= xCnt) {
					index -= xCnt;
					continue;
				}

				for (ll y = 0; y <= N-1; y++) {
					ll z = sum - x - y;
					ll yCnt = (0 <= z && z <= N-1);
					if (index >= yCnt) {
						index -= yCnt;
						continue;
					}
					assert(index == 0 && yCnt == 1);
					return vector<int>({int(x),int(y),int(z)});
				}
				assert(false);
			}
			assert(false);
		}
		assert(false);
	}
};

void my_tests() {
	// FILL IN TESTS
	IterateOverACube().findCell(int(1e6), int(1e18)-1);
	int N = int(1e2);
	int ind = 0;
	for (int sum = 0; sum <= 3 * N - 3; sum++) {
		for (int x = 0; x <= N-1; x++) {
			for (int y = 0; y <= N-1; y++) {
				for (int z = 0; z <= N-1; z++) {
					if (x + y + z == sum) {
						auto v = IterateOverACube().findCell(N, ind);
						assert(v[0] == x && v[1] == y && v[2] == z);
						ind++;
					}
				}
			}
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::IterateOverACube;

// vim:ft=cpp
