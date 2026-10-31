#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class DogsInAGrid {

public:

	vector <string> construct(int N) {
		const int W = 40;
		const int R_W = W + (W-2) + (W-2);
		std::vector<std::string> grid;
		char cur = 'D';
		grid.push_back(string(W, 'D'));

		while (N > 0) {
			cur ^= 'D' ^ 'G';
			if (N >= R_W) {
				grid.push_back(string(W, 'O'));
				grid.push_back(string(W, cur));
				N -= R_W;
				continue;
			}
			if (N <= 19) {
				grid.push_back(string(W, 'O'));
				grid.back()[0] = cur;
				for (int z = 0; z < N; z ++) {
					cur ^= 'D' ^ 'G';
					grid.back()[2 * z + 2] = cur;
				}
				break;
			}
			int excess = R_W - N;
			grid.push_back(string(W, 'O'));
			if (excess % 3 != 0) {
				excess -= 1;
				grid.back().front() = cur;
			}
			if (excess % 3 != 0) {
				excess -= 1;
				grid.back().back() = cur;
			}
			for (int z = 0; z < excess / 3; z++) {
				grid.back()[z+1] = cur;
			}
			grid.push_back(string(W, cur));
			break;
		}
		assert(grid.size() <= 40);
		return grid;
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int N = 0; N <= 2222; N++) {
		auto G = DogsInAGrid().construct(N);
		assert(int(G.size()) <= 40);
		assert(int(G[0].size()) <= 40);
		for (int i = 0; i < int(G.size()); i++) {
			assert(G[i].size() == G[0].size());
		}
		int cnt = 0;
		for (int x = 0; x < int(G.size()); x++) {
			for (int y = 0; y < int(G[0].size()); y++) {
				if (G[x][y] != 'O') continue;
				for (int dx = -1; dx <= 1; dx++) {
					for (int dy = -1; dy <= 1; dy++) {
						if (dx == 0 && dy == 0) continue;
						if (dx && (x-1 == -1 || x+1 == int(G.size()))) continue;
						if (dy && (y-1 == -1 || y+1 == int(G[0].size()))) continue;
						if (G[x-dx][y-dy] == 'D' && G[x+dx][y+dy] == 'G') cnt++;
					}
				}
			}
		}
		cerr << "Checked " << N << ' ' << cnt << '\n';
		for (string s : G) cerr << s << '\n';
		assert(cnt == N);
	}
}

}  // namespace ecnerwala

using ecnerwala::DogsInAGrid;

// vim:ft=cpp
