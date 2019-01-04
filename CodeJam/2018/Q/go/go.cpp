#include<bits/stdc++.h>
using namespace std;

void solve20() {
	// Fill in a 9x3 at 500, 500
	// We'll just hit the squares (500, 500), (503, 500), (506, 500) until it works
	while (true) {
		for (int i = 0; i < 9; i += 3) {
			for (int j = 0; j < 3; j += 3) {
				printf("%d %d\n", 500 + i, 500 + j);
				fflush(stdout);
				int x, y; scanf(" %d %d", &x, &y);
				if (x == 0 && y == 0) {
					return;
				} else if (x == -1 && y == -1) {
					// Wrong answer, just exit
					exit(0);
				} else {
					assert(1 <= x && x <= 1000);
					assert(1 <= y && y <= 1000);
				}
			}
		}
	}
}

void solve200() {
	bool planted[50][50];
	memset(planted, false, sizeof(planted));

	// We'll just do a 15 * 14 block at (500, 500)
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 14; j++) {

			// Location to query, coerced into bounds
			int si = min(i + 1, 13), sj = min(j + 1, 12);
			assert(0 <= si - 1 && si + 1 < 15);
			assert(0 <= sj - 1 && sj + 1 < 14);

			si += 500;
			sj += 500;

			while (!planted[i][j]) {
				printf("%d %d\n", si, sj);
				fflush(stdout);
				int x, y; scanf(" %d %d", &x, &y);
				if (x == 0 && y == 0) {
					return;
				} else if (x == -1 && y == -1) {
					// Wrong answer, just exit
					exit(0);
				} else {
					assert(1 <= x && x <= 1000);
					assert(1 <= y && y <= 1000);
					assert(si - 1 <= x && x <= si + 1);
					assert(sj - 1 <= y && y <= sj + 1);

					x -= 500;
					y -= 500;

					planted[x][y] = true;
				}
			}
		}
	}
}

int main() {
	int T; scanf("%d", &T);

	for(int case_num = 1; case_num <= T; case_num ++) {
		int A; scanf("%d", &A);
		if (A == 20) {
			// We'll just print 9 x 3
			// Starting at 100, 100
			solve20();
		} else if (A == 200) {
			solve200();
		} else assert(false);
	}

	return 0;
}
