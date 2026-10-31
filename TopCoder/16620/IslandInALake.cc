#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class IslandInALake {

	int flood(vector<string>& country, int i, int j, char orig, char nxt, bool diag) {
		if (!(0 <= i && i < int(country.size()) && 0 <= j && j < int(country[i].size()) && country[i][j] == orig)) {
			return 0;
		}
		country[i][j] = nxt;
		int res = 1;
		for (int di = -1; di <= 1; di++) {
			for (int dj = -1; dj <= 1; dj++) {
				if (!diag && di && dj) continue;
				res += flood(country, i+di, j+dj, orig, nxt, diag);
			}
		}
		return res;
	}

public:

	int build(vector <string> country) {
		int N = int(country.size());
		int M = int(country[0].size());
		// first, flood fill the ocean
		flood(country,0,0,'.','!',false);
		//for (const string& s : country) { cerr << s << '\n'; } cerr << '\n';
		// mark land as bad
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (country[i][j] == '#') {
					for (int di = -1; di <= 1; di++) {
						for (int dj = -1; dj <= 1; dj++) {
							if (country[i+di][j+dj] == '.') {
								country[i+di][j+dj] = '!';
							}
						}
					}
				}
			}
		}
		//for (const string& s : country) { cerr << s << '\n'; } cerr << '\n';
		int ans = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				ans = max(ans, flood(country,i,j,'.','!',true));
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::IslandInALake;

// vim:ft=cpp
