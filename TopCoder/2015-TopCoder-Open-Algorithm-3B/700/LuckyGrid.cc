#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class LuckyGrid {

	static const int MAXN = 70;
	int N;

public:

	static const int MAXV = MAXN * MAXN;
	int V;
	vector<int> adj[MAXV]; // bipartite matching

	int prv[MAXV];

	bool vis[MAXV];

	bool dfs(int a) {
		if(vis[a]) return false;
		vis[a] = true;
		for(int n : adj[a]) {
			if(prv[n] == -1) {
				prv[n] = a;
				return true;
			}
			int b = prv[n];
			prv[n] = a;
			if(dfs(b)) return true;
			prv[n] = b;
		}
		return false;
	}

	int findMinimumSum(vector <string> grid) {
		N = int(grid.size());
		if(N == 1) {
			// (1, 0) or (0, 1)
			if(grid[0][0] == '.') return 4;
			else return grid[0][0] - '0';
		} else if(N == 8 || N == 14 || N == 17 || N == 66) {
			int r, c;
			if(N == 8) {
				// (4, 4) or (3, 5)
				r = 3, c = 4;
			} else if(N == 14) {
				// (8, 6), (7, 7)
				r = 7, c = 6;
			} else if(N == 17) {
				// (15, 2), (14, 3)
				r = 14, c = 2;
			} else if(N == 66) {
				// (6, 60), (5, 61)
				r = 5, c = 60;
			} else {
				r = 0, c = 0, assert(false);
			}
			assert(r + c + 1 == N);

			V = 0;
			for(int i = 0; i < N; i++) {
				for(int t = 0; t < r; t++) {
					for(int j = 0; j < N; j++) {
						if(grid[i][j] == '4' || grid[i][j] == '.') {
							adj[V].push_back(N * i + j);
						}
					}
					V++;
				}
			}
			for(int j = 0; j < N; j++) {
				for(int t = 0; t < c; t++) {
					for(int i = 0; i < N; i++) {
						if(grid[i][j] == '7' || grid[i][j] == '.') {
							adj[V].push_back(N * i + j);
						}
					}
					V++;
				}
			}

			assert(V == N * N - N);

			memset(prv, -1, sizeof(prv));

			for(int i = 0; i < V; i++) {
				memset(vis, 0, sizeof(vis));
				if(!dfs(i)) return -1;
			}

			int res = N * c * 7 + N * r * 4;

			for(int i = 0; i < N; i++) {
				// we want each row to have an extra 4 if possible
				for(int j = 0; j < N; j++) {
					if(grid[i][j] == '4' || grid[i][j] == '.') {
						adj[V].push_back(N * i + j);
					}
				}
				memset(vis, 0, sizeof(vis));
				if(dfs(V)) {
					res += 4;
				} else {
					res += 7;
				}
				V++;
			}

			return res;
		} else if(N == 11) {
			// (11, 0), (10, 1), (1, 10), (0, 11)
			return 1;
		} else {
			return -1;
		}
	}
};

// lucky number between 1, 7 * 68 = 476
// 4, 7, 44, 47, 74, 77, 444, 447, 474
// 4
// (1, 0) = 1
// 7
// (0, 1) = 1
// 44
// (11, 0) = 11
// (4, 4) = 8
// 47
// (10, 1) = 11
// (3, 5) = 8
// 74
// (1, 10) = 11
// (8, 6) = 14
// (15, 2) = 17
// 77
// (0, 11) = 11
// (7, 7) = 14
// (14, 3) = 17
// 444
// (6, 60) = 66
// 447
// (5, 61) = 66

// vim:ft=cpp
