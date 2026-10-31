#include <bits/stdc++.h>
using namespace std;

// if any are degree 4, it's bad
// otherwise, color in row-major order
namespace ecnerwala {

const int MOD = int(1e9)+7;

class Newgenerations {

public:

	int count(vector <string> field) {
		int R = int(field.size());
		assert(R);
		int C = int(field[0].size());
		assert(C);
		vector<pair<int, int>> special;
		for (int i = 1; i < R-1; i++) {
			for (int j = 1; j < C-1; j++) {
				if (field[i][j] == 'x' && (field[i-1][j] != '.') && (field[i+1][j] != '.') && (field[i][j-1] != '.') && (field[i][j+1] != '.')) {
					field[i][j] = char(-1-int(special.size()));
					special.emplace_back(i, j);
				}
			}
		}
		int S = int(special.size());
		vector<int> cnt(1 << S);

		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (field[i][j] == '*') {
					int v = 0;
					if (i > 0 && field[i-1][j] < 0) {
						v |= (1 << (-1-field[i-1][j]));
					}
					if (i < R-1 && field[i+1][j] < 0) {
						v |= (1 << (-1-field[i+1][j]));
					}
					if (j > 0 && field[i][j-1] < 0) {
						v |= (1 << (-1-field[i][j-1]));
					}
					if (j < C-1 && field[i][j+1] < 0) {
						v |= (1 << (-1-field[i][j+1]));
					}
					cnt[(1 << S) - 1 - v] ++;
				}
			}
		}

		for (int i = 0; i < S; i++) {
			for (int m = 0; m < (1 << S); m++) {
				if (m & (1 << i)) continue;
				cnt[m] += cnt[m | (1 << i)];
			}
		}

		vector<int> pow2;
		for (int i = 0, v = 1; i <= R*C; i++, v <<= 1, (v = (v >= MOD) ? (v - MOD) : v)) {
			pow2.push_back(v);
		}
		int res = 0;
		for (int m = 0; m < (1 << S); m++) {
			if (__builtin_popcount(m) & 1) {
				res -= pow2[cnt[m]];
				if (res < 0) res += MOD;
			} else {
				res += pow2[cnt[m]];
				if (res >= MOD) res -= MOD;
			}
		}
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::Newgenerations;

// vim:ft=cpp
