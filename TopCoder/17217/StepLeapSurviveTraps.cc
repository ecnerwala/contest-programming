#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class StepLeapSurviveTraps {

public:

	long long minDamage(int N, int J, int seed, int M) {

		std::vector<int64_t> T(N+1);
		T[0] = 0;
		int64_t state = seed;
		for (int i = 1; i <= N; i++) {
			state = (state * 1103515245 + 12345) % (int64_t(1) << 31);
			T[i] = 1 + (state % M);
		}

		std::vector<int64_t> dp(N+1);
		dp[0] = 0;
		std::vector<pair<int64_t, int>> q; q.reserve(N+1);
		q.push_back({dp[0], 0});
		int lo = 0;
		for (int i = 1; i <= N; i++) {
			while (lo < int(q.size()) && q[lo].second < i-J) lo++;
			dp[i] = q[lo].first + T[i];
			while (lo < int(q.size()) && dp[i] <= q.back().first) q.pop_back();
			q.push_back({dp[i], i});
		}
		return dp[N];
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::StepLeapSurviveTraps;

// vim:ft=cpp
