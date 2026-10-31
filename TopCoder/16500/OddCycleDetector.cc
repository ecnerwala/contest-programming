#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class OddCycleDetector {

	int N;
	vector<vector<int>> adj;
	vector<vector<int>> ans;

	vector<int> depth;
	void dfs(int cur, int prv) {
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
		}
	}

public:

	vector <int> detect(vector <string> G) {
		N = int(G.size());
		adj = vector<vector<int>>(N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (G[i][j] == 'Y') adj[i].push_back(j);
			}
		}
		ans = vector<vector<int>>(N);
		depth = vector<int>(N, -1);

		depth[0] = 0;
		dfs(0, -1);

		vector<int> real_ans;
		for (int i = 0; i < N; i++) {
			if (i) real_ans.push_back(-1);
			real_ans.insert(real_ans.end(), ans[i].begin(), ans[i].end());
		}
		return real_ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::OddCycleDetector;

// vim:ft=cpp
