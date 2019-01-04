#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class BoardCoveringDiv1 {

public:

	vector <string> make(int N, int R, int C) {
		if (N % 3 == 0) {
			return vector<string>{};
		}
		vector<string> res(N, string(N, '\0'));
		int rl = 0, rh = N;
		assert(rl <= R && R < rh);
		int cl = 0, ch = N;
		assert(cl <= C && C < ch);
		while (rl + 3 <= R) rl += 3;
		while (rh - 3 > R) rh -= 3;
		while (cl + 3 <= C) cl += 3;
		while (ch - 3 > C) ch -= 3;

		assert(rl <= R && R < rh);
		assert(cl <= C && C < ch);
		assert(rh - rl <= 5);
		assert(ch - cl <= 5);
		assert((rh - rl) % 3 == N % 3);
		assert((ch - cl) % 3 == N % 3);

		if (N % 3 == 1) {
			assert(rh - rl == 1 || rh - rl == 4);
			assert(ch - cl == 1 || ch - cl == 4);
			if (rh - rl == 1 && ch - cl == 1) {
				// we're good
			} else {
				assert(N >= 4);
				if (rh - rl == 1) {
					if (rl >= 3) {
						rl -= 3;
					} else {
						rh += 3;
					}
				}
				assert(rh - rl == 4);
				if (ch - cl == 1) {
					if (cl >= 3) {
						cl -= 3;
					} else {
						ch += 3;
					}
				}
				assert(ch - cl == 4);

				// now just solve the 4x4 square
				// corner pieces first
				res[rl+0][cl+0] = '0';
				res[rl+0][cl+1] = '0';
				res[rl+1][cl+0] = '0';

				res[rl+0][cl+2] = '1';
				res[rl+0][cl+3] = '1';
				res[rl+1][cl+3] = '1';

				res[rl+2][cl+0] = '1';
				res[rl+3][cl+0] = '1';
				res[rl+3][cl+1] = '1';

				res[rl+2][cl+3] = '0';
				res[rl+3][cl+2] = '0';
				res[rl+3][cl+3] = '0';

				// center next
				res[rl+1][cl+1] = '2';
				res[rl+1][cl+2] = '2';
				res[rl+2][cl+1] = '2';
				res[rl+2][cl+2] = '2';

				// fill in the middle except the correct square
				int ra = (R - rl < 2) ? 1 : 2;
				int ca = (C - cl < 2) ? 1 : 2;
				res[rl+ra][cl+ca] = "01"[(ra^ca) & 1];
				res[R][C] = '\0';
			}
		} else if (N % 3 == 2) {
			assert(rh - rl == 2 || rh - rl == 5);
			assert(ch - cl == 2 || ch - cl == 5);
			if (rh - rl == 2 && ch - cl == 2) {
				// we're just a square
				res[rl+0][cl+0] = '0';
				res[rl+0][cl+1] = '0';
				res[rl+1][cl+0] = '0';
				res[rl+1][cl+1] = '0';
				// unset R/C
				res[R][C] = '\0';
			} else if (rh - rl == 5 && ch - cl == 2) {
				// solve this area
				assert(R - rl == 2);
				res[rl+0][cl+0] = '0';
				res[rl+0][cl+1] = '0';
				res[rl+1][cl+0] = '0';
				res[rl+1][cl+1] = '0';
				res[rl+3][cl+0] = '0';
				res[rl+3][cl+1] = '0';
				res[rl+4][cl+0] = '0';
				res[rl+4][cl+1] = '0';

				assert(C == cl || C == cl + 1);
				int c2 = cl ^ (cl + 1) ^ C;
				res[rl+1][c2] = '1';
				res[rl+2][c2] = '1';
				res[rl+3][c2] = '1';
			} else if (rh - rl == 2 && ch - cl == 5) {
				assert(C - cl == 2);
				res[rl+0][cl+0] = '0';
				res[rl+1][cl+0] = '0';
				res[rl+0][cl+1] = '0';
				res[rl+1][cl+1] = '0';
				res[rl+0][cl+3] = '0';
				res[rl+1][cl+3] = '0';
				res[rl+0][cl+4] = '0';
				res[rl+1][cl+4] = '0';

				assert(R == rl || R == rl + 1);
				int r2 = rl ^ (rl + 1) ^ R;
				res[r2][cl+1] = '1';
				res[r2][cl+2] = '1';
				res[r2][cl+3] = '1';
			} else if (rh - rl == 5 && ch - cl == 5) {
				assert(R - rl == 2);
				assert(C - cl == 2);
				res[rl+0][cl+0] = '0';
				res[rl+0][cl+1] = '0';
				res[rl+0][cl+2] = '0';

				res[rl+1][cl+0] = '1';
				res[rl+1][cl+1] = '1';
				res[rl+1][cl+2] = '1';

				res[rl+0][cl+3] = '2';
				res[rl+1][cl+3] = '2';
				res[rl+2][cl+3] = '2';

				res[rl+0][cl+4] = '0';
				res[rl+1][cl+4] = '0';
				res[rl+2][cl+4] = '0';

				res[rl+3][cl+2] = '1';
				res[rl+3][cl+3] = '1';
				res[rl+3][cl+4] = '1';

				res[rl+4][cl+2] = '0';
				res[rl+4][cl+3] = '0';
				res[rl+4][cl+4] = '0';

				res[rl+2][cl+1] = '2';
				res[rl+3][cl+1] = '2';
				res[rl+4][cl+1] = '2';

				res[rl+2][cl+0] = '0';
				res[rl+3][cl+0] = '0';
				res[rl+4][cl+0] = '0';

			} else assert(false);
		} else assert(false);

		for (int i = 0; i < rl; i ++) {
			// vertical strips all the way
			for (int j = 0; j < N; j++) {
				res[i][j] = "89"[(((i-0) / 3) ^ j) & 1];
			}
		}
		for (int i = rh; i < N; i ++) {
			for (int j = 0; j < N; j++) {
				res[i][j] = "89"[(((i-rh) / 3) ^ j) & 1];
				res[i][j] = "89"[(((i-rh) / 3) ^ j) & 1];
				res[i][j] = "89"[(((i-rh) / 3) ^ j) & 1];
			}
		}

		for (int i = rl; i < rh; i++) {
			for (int j = 0; j < cl; j ++) {
				res[i][j] = "67"[(((j-0) / 3) ^ i) & 1];
			}
			for (int j = ch; j < N; j ++) {
				res[i][j] = "67"[(((j-ch) / 3) ^ i) & 1];
			}
		}

		assert(res[R][C] == '\0');
		res[R][C] = '#';
		for (const string& s : res) {
			//cerr << s << '\n';
			for (char c : s) assert(c);
		}
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::BoardCoveringDiv1;

// vim:ft=cpp
