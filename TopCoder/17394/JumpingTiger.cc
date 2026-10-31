#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, -1, 0, 1};

class JumpingTiger {

public:

	int travel(vector <string> G) {
		int R = int(G.size());
		int C = int(G[0].size());
		int ST = -1, EN = -1;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (G[i][j] == 'T') {
					ST = i*C + j;
				} else if (G[i][j] == 'L') {
					EN = i*C+j;
				}
			}
		}
		assert(ST != -1 && EN != -1);
		std::vector<int> dist(R*C, -1);
		min_pq<std::pair<int, int>> pq;
		pq.push({dist[ST] = 0, ST});
		while (!pq.empty()) {
			int d = pq.top().first, cur = pq.top().second; pq.pop();
			if (d != dist[cur]) continue;
			int x = cur/C, y = cur%C;
			for (int z = 0; z < 4; z++) {
				for (int i = 1; true; i++) {
					int nx = x + mx[z] * i;
					int ny = y + my[z] * i;
					if (nx < 0 || nx >= R || ny < 0 || ny >= C) break;
					if (G[nx][ny] == '#') continue;
					if (G[nx-mx[z]][ny-my[z]] == '#') continue;
					int nd = d + 1 + (i > 1);
					int nxt = nx*C + ny;
					if (dist[nxt] == -1 || nd < dist[nxt]) {
						pq.push({dist[nxt] = nd, nxt});
					}
				}
			}
		}
		return dist[EN];
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::JumpingTiger;

// vim:ft=cpp
