#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class MultiWordSearch {

public:

	int countTimes(vector <string> grid, string word) {
		int N = int(grid.size());
		int M = int(grid[0].size());
		int L = int(word.size());
		int ans = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j+L <= M; j++) {
				bool v = true;
				for (int k = 0; k < L; k++) {
					v = v && word[k] == grid[i][j+k];
				}
				ans += v;
			}
		}
		for (int i = 0; i+L <= N; i++) {
			for (int j = 0; j < M; j++) {
				bool v = true;
				for (int k = 0; k < L; k++) {
					v = v && word[k] == grid[i+k][j];
				}
				ans += v;
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MultiWordSearch;

// vim:ft=cpp
