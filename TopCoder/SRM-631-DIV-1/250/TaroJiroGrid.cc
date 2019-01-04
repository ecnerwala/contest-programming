#include <bits/stdc++.h>
using namespace std;

class TaroJiroGrid {

	bool good(const vector<string> &g) {
		int N = int(g.size());
		assert(N);
		assert(N % 2 == 0);
		int M = int(g[0].size());
		for (int c = 0; c < M; c++) {
			int cons = 0;
			char prv = '\0';
			for (int r = 0; r < N; r++) {
				char cur = g[r][c];
				if (cur != prv) {
					prv = cur;
					cons = 0;
				}
				cons ++;
				if (cons > N / 2) return false;
			}
		}
		return true;
	}

public:

	int getNumber(vector <string> grid) {
		if (good(grid)) return 0;
		int N = int(grid.size());
		int M = int(grid[0].size());
		string W(size_t(M), 'W');
		string B(size_t(M), 'B');
		for (int i = 0; i < int(grid.size()); i++) {
			swap(W, grid[size_t(i)]);
			if (good(grid)) return 1;
			swap(W, grid[size_t(i)]);

			swap(B, grid[size_t(i)]);
			if (good(grid)) return 1;
			swap(B, grid[size_t(i)]);
		}
		return 2;
	}
};

// vim:ft=cpp
