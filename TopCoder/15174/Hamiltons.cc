#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Hamiltons {

public:

	vector <int> construct(int N, int L) {
		vector<vector<int>> adj(N, vector<int>(N, L));
		for (int i = 0; i < N; i++) {
			adj[i][(i+1)%N] = adj[(i+1)%N][i] = 1;
		}
		adj[4][5] = adj[5][4] = L;
		// alternate better cycle
		// (L+1) / 3
		adj[0][2] = adj[2][0] = (L+2 + 0) / 4;
		adj[2][4] = adj[4][2] = (L+2 + 1) / 4;
		adj[3][1] = adj[1][3] = (L+2 + 2) / 4;
		adj[1][5] = adj[5][1] = (L+2 + 3) / 4;
#ifdef ECNERWALA_MACHINE
		{
			vector<int> perm(N); iota(perm.begin(), perm.end(), 0);
			const int INF = 1e9;
			pair<int, int> best_cyc{INF, INF};
			int best_path = INF;
			do {
				int cur_path = 0;
				for (int i = 0; i+1 < N; i++) {
					cur_path += adj[perm[i]][perm[i+1]];
				}
				best_path = min(best_path, cur_path);
				best_cyc = min(best_cyc, {cur_path + adj[perm[N-1]][perm[0]], cur_path});
			} while (next_permutation(perm.begin(), perm.end()));
			cerr << best_cyc.first << ' ' << best_cyc.second << ' ' << best_path << '\n';
			assert((best_cyc.second - best_path) * 2 >= L);
		}
#endif

		vector<int> res; res.reserve(N*N);
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				assert(adj[i][j] == adj[j][i]);
				res.push_back(adj[i][j]);
			}
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
	Hamiltons().construct(6, 1000);
	for (int N = 6; N <= 14; N++) {
		for (int L : {10, 11, 12, 13, 14, 15, 16, 1000}) {
			cerr << "N=" << N << ' ' << "L=" << L << '\n';
			Hamiltons().construct(N, L);
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::Hamiltons;

// vim:ft=cpp
