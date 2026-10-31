#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class OneBattleship {

public:

	vector <string> hit(vector <string> grid) {
		std::array<int, 3> cnt; cnt.fill(0);
		for (int i = 0; i < int(grid.size()); i++) {
			for (int j = 0; j < int(grid[i].size()); j++) {
				if (grid[i][j] == '.') {
					cnt[(i+j)%3]++;
				}
			}
		}
		int v = int(std::min_element(cnt.begin(), cnt.end()) - cnt.begin());
		for (int i = 0; i < int(grid.size()); i++) {
			for (int j = 0; j < int(grid[i].size()); j++) {
				if (grid[i][j] == '.' && (i+j)%3 == v) {
					grid[i][j] = '*';
				}
			}
		}
		return grid;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::OneBattleship;

// vim:ft=cpp
