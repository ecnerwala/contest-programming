#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

const int dx[4] = {-1, 0, 1, 0};
const int dy[4] = {0, 1, 0, -1};

class PoisonedSwamp {

public:

	string cross(vector <string> swamp) {
		int N = int(swamp.size());
		int M = int(swamp[0].size());
		vector<int> vis(N*M*10);
		vector<int> q; q.reserve(N*M*10);
		vis[0] = true;
		q.push_back(0);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			int x = cur / 10 / M;
			int y = cur / 10 % M;
			int p = cur % 10;
			for (int d = 0; d < 4; d++) {
				int nx = x + dx[d];
				int ny = y + dy[d];
				if (0 <= nx && nx < N && 0 <= ny && ny < M) {
					char c = swamp[nx][ny];
					int np;
					if (c == 'S') {
						np = 0;
					} else if ('1' <= c && c <= '9') {
						np = p + (c - '0');
					} else if (c == '.') {
						np = p;
					} else if (c == '#') {
						np = 10;
					} else assert(false);

					if (np < 10) {
						if (nx == N-1 && ny == M-1) return "possible";
						int nxt = (nx * M + ny) * 10 + np;
						if (!vis[nxt]) {
							vis[nxt] = true;
							q.push_back(nxt);
						}
					}
				}
			}
		}

		return "impossible";
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::PoisonedSwamp;

// vim:ft=cpp
