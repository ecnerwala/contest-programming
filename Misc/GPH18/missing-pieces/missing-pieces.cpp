#include<bits/stdc++.h>
using namespace std;

struct sq {
	int x, y;
	int v;
};
using hexomino = sq[6];

hexomino pieces[16] = {
	// First 4 are special ones:
	{{0, 0, 0}, {0, 1, 3}, {0, 2, 2}, {0, 3, 5}, {0, 4, 1}, {0, 5, 0}, },
	{{0, 0, 0}, {1, 0, 6}, {1, 1, 3}, {2, 0, 0}, {3, 0, 1}, {4, 0, 4}, },
	{{0, 0, 0}, {1, 0, 0}, {2, 0, 5}, {3, 0, 6}, {4, 0, 4}, {5, 0, 1}, },
	{{0, 0, 0}, {0, 1, 6}, {0, 2, 3}, {0, 3, 4}, {1, 0, 2}, {1, 3, 0}, },

	// Now we go roughly by row
	{{0, 0, 1}, {0, 3, 3}, {1, 0, 0}, {1, 1, 4}, {1, 2, 2}, {1, 3, 0}, }, // U
	{{0, 0, 2}, {0, 1, 3}, {0, 2, 0}, {1, 0, 6}, {1, 1, 0}, {1, 2, 1}, }, // Square
	{{0, 0, 4}, {0, 1, 5}, {0, 2, 1}, {0, 3, 0}, {0, 4, 0}, {1, 0, 3}, }, // |--
	{{0, 0, 5}, {1, 0, 0}, {1, 1, 6}, {1, 2, 0}, {1, 3, 2}, {1, 4, 3}, }, // |__

	{{0, 1, 4}, {1, 1, 0}, {1, 2, 1}, {1, 3, 6}, {2, 0, 0}, {2, 1, 5}, }, // seat
	{{0, 0, 0}, {0, 1, 4}, {1, 1, 2}, {2, 0, 6}, {2, 1, 3}, {3, 1, 0}, }, // F
	{{0, 0, 0}, {1, 0, 2}, {1, 1, 5}, {1, 2, 6}, {2, 1, 1}, {2, 2, 0}, }, // |-o
	{{0, 0, 4}, {1, 0, 0}, {2, 0, 1}, {3, 0, 0}, {3, 1, 2}, {3, 2, 5}, }, // L

	{{0, 0, 6}, {0, 1, 0}, {1, 1, 1}, {2, 1, 4}, {3, 1, 0}, {4, 1, 3}, }, // -|
	{{0, 0, 1}, {1, 0, 0}, {2, 0, 4}, {2, 1, 6}, {3, 0, 0}, {4, 0, 5}, }, // |-
	{{0, 0, 1}, {0, 1, 2}, {1, 0, 0}, {1, 1, 0}, {2, 1, 3}, {3, 1, 4}, }, // ^|
	{{0, 2, 3}, {0, 3, 5}, {1, 0, 0}, {1, 1, 1}, {1, 2, 0}, {1, 3, 4}, }, // _o
};

int g[4][6][6];
bool b[4][6][6];
bool r[4][6][6];
bool c[4][6][6];

bool can_fill(const hexomino &h, int d, int x, int y) {
	for (const sq &s : h) {
		if (x + s.x >= 6) return false;
		if (y + s.y >= 6) return false;
		if (b[d][x + s.x][y + s.y]) return false;
		if (s.v) {
			if (r[d][x + s.x][s.v - 1]) return false;
			if (c[d][y + s.y][s.v - 1]) return false;
		}
	}
	return true;
}

void fill(const hexomino &h, int d, int x, int y) {
	for (const sq &s : h) {
		b[d][x + s.x][y + s.y] = true;
		if (s.v) {
			r[d][x + s.x][s.v-1] = true;
			c[d][y + s.y][s.v-1] = true;
		}
		g[d][x+s.x][y+s.y] = s.v;
	}
}

void unfill(const hexomino &h, int d, int x, int y) {
	for (const sq &s : h) {
		b[d][x + s.x][y + s.y] = false;
		if (s.v) {
			r[d][x + s.x][s.v-1] = false;
			c[d][y + s.y][s.v-1] = false;
		}
	}
}

int pn[16], px[16], py[16];
bool used[16];
void dfs(int l=0, int p=0) {
	if (l == 16) {
		for (int i = 0; i < l; i++) {
			cerr << pn[i] << ' ' << px[i] << ' ' << py[i] << '\n';
		}
		for (int d = 0; d < 4; d++) {
			int cnt = 0;
			for (int i = 0; i < 6; i++) {
				for (int j = 0; j < 6; j++) {
					cerr << (b[d][i][j] ? char('0'+g[d][i][j]) : '_');
					cnt += b[d][i][j];
				}
				cerr << '\n';
			}
			cerr << '\n';
			assert(cnt == 24);
		}
		cerr << '\n';
		return;
	}

	if (l % 4 == 0) p = l / 4;

	// not enough things left
	if (16 - p < 4 - (l % 4)) return;

	// Upper left constraint
	if (!used[p] && ((l % 4 && p >= 4) || (l % 4 == 0 && p == l / 4))) {
		// take current

		for (int x = 0; x < 6; x++) {
			for (int y = 0; y < 6; y++) {
				if (l % 4 == 0) {
					if (x || y) continue;
				}
				if (can_fill(pieces[p], l / 4, x, y)) {
					fill(pieces[p], l / 4, x, y);
					used[p] = true;

					pn[l] = p;
					px[l] = x;
					py[l] = y;
					dfs(l+1, p+1);

					used[p] = false;
					unfill(pieces[p], l / 4, x, y);
				}
			}
		}
	}

	// skip current
	if (l % 4) dfs(l, p+1);
}

int main() {
	dfs();
	return 0;
}
