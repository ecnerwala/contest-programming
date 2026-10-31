#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class AtLeastKDays {

public:

	long long sumOfMinCosts(vector <string> costs, int minDays) {
		int N = int(costs.size());
		vector<int64_t> d(N*N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (costs[i][j] == '-') d[i*N+j] = -1;
				else d[i*N+j] = costs[i][j] - '0';
			}
		}

		auto mul = [&](const vector<int64_t>& a, const vector<int64_t>& b) -> vector<int64_t> {
			vector<int64_t> res(N*N);
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					res[i*N+j] = -1;
					for (int k = 0; k < N; k++) {
						if (a[i*N+k] == -1 || b[k*N+j] == -1) continue;
						int64_t cnd = a[i*N+k] + b[k*N+j];
						if (res[i*N+j] == -1 || cnd < res[i*N+j]) res[i*N+j] = cnd;
					}
				}
			}
			return res;
		};

		vector<int64_t> dany = d;
		for (int i = 0; i < N; i++) dany[i*N+i] = 0;
		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					dany[i*N+j] = min(dany[i*N+j], dany[i*N+k] + dany[k*N+j]);
				}
			}
		}
		while (minDays) {
			if (minDays & 1) {
				dany = mul(dany, d);
			}
			d = mul(d, d);
			minDays >>= 1;
		}
		return std::accumulate(dany.begin(), dany.end(), int64_t(0));
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::AtLeastKDays;

// vim:ft=cpp
