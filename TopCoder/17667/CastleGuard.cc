#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class CastleGuard {

public:

	vector<long long> walk(int N, int R, vector <int> commands) {
		std::vector<long long> ans_single(N);
		int cur = 0;
		for (int v : commands) {
			if (v > 0) {
				for (int i = 0; i < v % N; i++) {
					cur++;
					if (cur == N) cur -= N;
					ans_single[cur]++;
				}
				for (auto& x : ans_single) x += v / N;
			} else {
				v = -v;
				for (int i = 0; i < v % N; i++) {
					cur--;
					if (cur == -1) cur += N;
					ans_single[cur]++;
				}
				for (auto& x : ans_single) x += v / N;
			}
		}

		std::vector<long long> ans_tot(N);
		ans_tot[0]++;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				ans_tot[(cur*i+j)%N] += ans_single[j] * (R / N + (i < R % N));
			}
		}
		return ans_tot;
	}
};

void my_tests() {
	// FILL IN TESTS
	auto res = CastleGuard().walk(500, int(1e7), std::vector<int>(500, int(1e9)));
	for (auto v : res) { cerr << v << ' '; } cerr << '\n';
}

}  // namespace ecnerwala

using ecnerwala::CastleGuard;

// vim:ft=cpp
