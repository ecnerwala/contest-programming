#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Coastlines {

public:

	int longest(vector <string> G) {
		int N = int(G.size());
		int M = int(G[0].size());
		vector<vector<bool>> vis(N, vector<bool>(M, false));
		int ans = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (G[i][j] == '.') continue;
				if (vis[i][j]) continue;
				stack<pair<int, int>> q;
				int cnt = 0;
				vis[i][j] = true;
				q.emplace(i,j);
				while (!q.empty()) {
					int x,y; tie(x,y) = q.top(); q.pop();
					const int mx[4] = {-1, 0, 1, 0};
					const int my[4] = {0, -1, 0, 1};
					for (int d = 0; d < 4; d++) {
						int nx = x + mx[d], ny = y + my[d];
						if (0 <= nx && nx < N && 0 <= ny && ny < M && G[nx][ny] != '.') {
							if (!vis[nx][ny]) {
								vis[nx][ny] = true;
								q.emplace(nx, ny);
							}
						} else {
							cnt++;
						}
					}
				}
				ans = max(ans, cnt);
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::Coastlines;

// vim:ft=cpp
