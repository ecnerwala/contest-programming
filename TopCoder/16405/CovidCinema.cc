#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class CovidCinema {

public:

	vector <string> seat(int R, int C, int A, int B) {
		if (R < C) {
			vector<string> res = seat(C, R, A, B);
			if (res.empty()) return res;
			vector<string> transpose_res(R, string(C, '\0'));
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					transpose_res[i][j] = res[j][i];
				}
			}
			return transpose_res;
		}
		assert(R >= C);
		if (A < B) {
			vector<string> res = seat(R, C, B, A);
			if (res.empty()) return res;
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					if (res[i][j] != '.') res[i][j] = 'A' ^ 'B' ^ res[i][j];
				}
			}
			return res;
		}
		assert(A >= B);

		assert(A > 0 && B > 0);
		int extra = R * C - A - B;
		if (extra <= 0) return {};
		if (extra >= C) {
			vector<string> res(R, string(C, '.'));
			for (int i = 0; i < A; i++) {
				res[i/C][i%C] = 'A';
			}
			for (int i = R*C-1; i >= R*C-B; i--) {
				res[i/C][i%C] = 'B';
			}
			return res;
		}

		assert(extra < C);
		if (B <= extra * (extra-1) / 2) {
			vector<string> res(R, string(C, 'A'));
			int curX = 0, curY = 0;
			for (int i = 0; i < B + extra; i++) {
				res[curX][curY] = (i >= B ? '.' : 'B');
				if (curX == 0) {
					curX = curY+1;
					curY = 0;
				} else {
					curX--, curY++;
				}
			}
			return res;
		}
		return {};
	}
};

void my_tests() {
	auto test = [&](int R, int C, int A, int B) -> void {
		cerr << R << ' ' << C << ' ' << A << ' ' << B << '\n';
		vector<string> res = CovidCinema().seat(R, C, A, B);
		for (string s : res) cerr << s << '\n';
		if (res.empty()) return;
		assert(int(res.size()) == R);
		for (int i = 0; i < R; i++) {
			assert(int(res[i].size()) == C);
		}
		int cA = 0, cB = 0;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (res[i][j] == '.') ;
				else if (res[i][j] == 'A') cA++;
				else if (res[i][j] == 'B') cB++;
				else assert(false);
			}
		}
		assert(cA == A && cB == B);

		auto is_bad = [](char a, char b) { return (a == 'A' && b == 'B') || (a == 'B' && b == 'A'); };
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (i) assert(!is_bad(res[i-1][j], res[i][j]));
				if (j) assert(!is_bad(res[i][j-1], res[i][j]));
			}
		}
	};
	// FILL IN TESTS
	
	test(10, 10, 6, 100 - 6 - 4);
	test(50, 10, 243, 247);
	test(10, 50, 247, 243);
	
	for (int r = 1; r <= 10; r++) {
		for (int c = 1; c <= 5; c++) {
			for (int a = 1; a <= r * c; a++) {
				for (int b = 1; b <= r * c; b++) {
					test(r, c, a, b);
				}
			}
		}
	}
}

}  // namespace ecnerwala

using ecnerwala::CovidCinema;

// vim:ft=cpp
