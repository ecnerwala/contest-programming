#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, 1, 0, -1};

class TombExplorer {

public:

	int minimumDigging(vector <string> G) {
		int N = int(G.size());
		assert(N);
		int M = int(G[0].size());
		assert(M);
		int C = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (G[i][j] != '.') continue;
				stack<pair<int, int>> q;
				char v = char('0' + C); C++;

				G[i][j] = v;
				q.emplace(i, j);

				while (!q.empty()) {
					int x = q.top().first, y = q.top().second;
					q.pop();
					for (int d = 0; d < 4; d++) {
						int nx = x + mx[d], ny = y + my[d];
						if (0 <= nx && nx < N && 0 <= ny && ny < M && G[nx][ny] == '.') {
							G[nx][ny] = v;
							q.emplace(nx, ny);
						}
					}
				}
			}
		}

		vector<vector<int>> dists(C, vector<int>(C, N + M + 1));
		for (int i = 0; i < C; i++) dists[i][i] = 0;

		for (int x1 = 0; x1 < N; x1++) {
			for (int y1 = 0; y1 < M; y1++) {
				int t1 = G[x1][y1] - '0';
				if (!(0 <= t1 && t1 < C)) continue;
				for (int x2 = 0; x2 < N; x2++) {
					for (int y2 = 0; y2 < M; y2++) {
						int t2 = G[x2][y2] - '0';
						if (!(0 <= t2 && t2 < C)) continue;
						if (t1 != t2) {
							dists[t1][t2] = min(dists[t1][t2], abs(x1 - x2) + abs(y1 - y2) - 1);
						}
					}
				}
			}
		}

		//for (int i = 0; i < C; i++) { for (int j = 0; j < C; j++) { cerr << dists[i][j] << ' '; } cerr << '\n'; } cerr << '\n';

		for (int k = 0; k < C; k++) {
			for (int i = 0; i < C; i++) {
				for (int j = 0; j < C; j++) {
					dists[i][j] = min(dists[i][j], dists[i][k] + dists[k][j]);
				}
			}
		}

		//for (int i = 0; i < C; i++) { for (int j = 0; j < C; j++) { cerr << dists[i][j] << ' '; } cerr << '\n'; } cerr << '\n';

		int ans = C * (N+M+1);
		vector<int> perm(C);
		iota(perm.begin(), perm.end(), 0);
		do {
			int val = 0;
			for (int i = 0; i+1 < int(perm.size()); i++) {
				val += dists[perm[i]][perm[i+1]];
			}
			ans = min(ans, val);
		} while (next_permutation(perm.begin(), perm.end()));

		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::TombExplorer;

// vim:ft=cpp
