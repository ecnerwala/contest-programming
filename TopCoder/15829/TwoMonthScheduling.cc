#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class TwoMonthScheduling {

public:

	int minimumMonths(int workers, vector <int> firstMonth0, vector <int> firstMonth1, vector <int> secondMonth0, vector <int> secondMonth1) {
		vector<pair<ll, ll>> jobs;
		for (int i1 = 0; i1 < int(firstMonth1.size()); i1++) {
			for (int i0 = 0; i0 < int(firstMonth0.size()); i0++) {
				jobs.emplace_back(min(workers, firstMonth0[i0] ^ firstMonth1[i1]), min(workers, secondMonth0[i0] ^ secondMonth1[i1]));
			}
		}
		int N = int(jobs.size());
		vector<ll> pref1(N+1);
		vector<ll> pref2(N+1);
		for (int i = 0; i < N; i++) {
			pref1[i+1] = pref1[i] + jobs[i].first;
			pref2[i+1] = pref2[i] + jobs[i].second;
		}
		vector<vector<int>> dist(N+1, vector<int>(N+1, 2*N+3));
		vector<pair<int, int>> q;
		dist[0][0] = 0;
		q.push_back({0,0});
		for (int d = 0; true; d++) {
			vector<pair<int, int>> nq;
			for (int z = 0; z < int(q.size()); z++) {
				int i = q[z].first, j = q[z].second;
				if (dist[i][j] != d) continue;
				if (j > i) {
					if (d < dist[i][j-1]) {
						dist[i][j-1] = d;
						q.push_back({i, j-1});
					}
				}

				ll v = workers - (pref2[j] - pref2[i]);
				if (v < 0) continue;

				if (j == N) {
					return d+1;
				}
				int k = int(upper_bound(pref1.begin(), pref1.end(), v + pref1[j]) - pref1.begin()) - 1;
				assert(k >= j);
				if (d+1 < dist[j][k]) {
					dist[j][k] = d+1;
					nq.push_back({j, k});
				}
			}
			q = std::move(nq);
		}
	}
};

void my_tests() {
	// FILL IN TESTS
	cerr << TwoMonthScheduling().minimumMonths(int(400), vector<int>(500, 1), vector<int>(10, 0), vector<int>(500, 1), vector<int>(10, 0)) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::TwoMonthScheduling;

// vim:ft=cpp
