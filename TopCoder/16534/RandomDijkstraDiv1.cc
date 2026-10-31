#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class RandomDijkstraDiv1 {

public:

	double solve(int N, vector <int> G) {
		vector<int> dist = G;
		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					dist[i*N+j] = min(dist[i*N+j], dist[i*N+k] + dist[k*N+j]);
				}
			}
		}
		// distances from 0
		dist.resize(N);

		vector<uint32_t> preds(N);
		vector<uint32_t> succs(N);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int cnd = dist[i] + G[i*N+j];
				assert(cnd >= dist[j]);
				if (cnd == dist[j]) {
					preds[j] |= (1u << i);
					succs[i] |= (1u << j);
				}
			}
			assert(succs[i] & (1u << i));
		}

		vector<vector<pair<int, double>>> dp(1<<N);
		dp[0].push_back({0, 1.});
		mt19937 mt(48);
		for (int m = 0; m < (1 << N); m ++) {
			{
				shuffle(dp[m].begin(), dp[m].end(), mt);
				sort(dp[m].begin(), dp[m].end());
				int L = -1;
				for (int i = 0; i < int(dp[m].size()); i++) {
					if (L >= 0 && dp[m][i].first == dp[m][L].first) {
						dp[m][L].second += dp[m][i].second;
					} else {
						L++;
						dp[m][L] = dp[m][i];
					}
				}
				dp[m].resize(L+1);
			}
			if (m == (1 << N) - 1) break;
			for (auto it : dp[m]) {
				int c = it.first; double v = it.second;
				v /= (N - __builtin_popcount(m));
				for (int a = (1 << N) - 1 - m; a; a &= a-1) {
					int i = __builtin_ctz(a);
					assert(!(m & (1 << i)));
					int nm = m | (1 << i);
					if (i == 0 || (c & preds[i])) {
						// then we're correct
						dp[nm].push_back({c | (1 << i), v});
					} else {
						dp[nm].push_back({c, v});
					}
				}
				if (c == 0) break;
			}
		}
		double ans = 0;
		for (auto it : dp[(1<<N) - 1]) {
			int c = it.first; double v = it.second;
			bool good = true;
			for (int i = 0; i < N; i++) {
				if (!(c & preds[i])) {
					good = false;
					break;
				}
			}
			if (good) ans += v;
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	int N = 14;

	cerr << RandomDijkstraDiv1().solve(N, vector<int>(N*N, 0)) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::RandomDijkstraDiv1;

// vim:ft=cpp
