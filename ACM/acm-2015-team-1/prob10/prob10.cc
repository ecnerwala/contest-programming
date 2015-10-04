#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

typedef complex<int> pt;
#define X real()
#define Y imag()

const int INF = 1e9;
const pt BAD(INF, INF);

// represents a map from a -> ra + s
struct mp {
	pt r, s;

	mp(pt rot, pt shift) : r(rot), s(shift) {}

	pt operator () (pt a) {
		return r * a + s;
	}

	mp operator () (mp a) {
		// r * (a.r * x + a.s) + s
		return mp(r * a.r, r * a.s + s);
	}
};

// powers of i
const pt un[4] = { pt(1, 0), pt(0, 1), pt(-1, 0), pt(0, -1) };

const int MAXR = 100;
const int MAXC = 100;

int R, C;
char grid[MAXR][MAXC];

int J;
pt ori[20]; // orientation

pt coords[MAXR][MAXC];

bool flood(pt cur, mp tra, char prv) {
	if(coords[cur.X][cur.Y] == INF) {
		coords[cur.X][cur.Y] = tra(cur);
		for(int d = 0; d < 4; d++) {
			pt nxt = cur + un[d];
			mp ntra = tra;
			if(nxt.X < 0 || nxt.X >= R || nxt.Y < 0 || nxt.Y >= C) continue;
			if(grid[nxt.X][nxt.Y] == '.') continue;
			if('0' <= grid[cur.X][cur.Y] && grid[cur.X][cur.Y] <= '9') {
				int j = grid[cur.X][cur.Y] - '0';
				assert('a' <= grid[nxt.X][nxt.Y] && grid[nxt.X][nxt.Y] <= 'z');
				if(grid[nxt.X][nxt.Y] == prv) {
					ntra = tra;
				} else if(grid[nxt.X][nxt.Y] < prv) {
					ntra = mp(ori[j], tra(cur) - ori[j] * tra(cur))(tra);
				} else {
					ntra = mp(conj(ori[j]), tra(cur) - conj(ori[j]) * tra(cur))(tra);
				}
			} else {
				ntra = tra;
			}
			if(!flood(nxt, ntra, grid[cur.X][cur.Y])) return false;
		}
		return true;
	} else if(coords[cur.X][cur.Y] != tra(cur)) {
		return false;
	} else {
		return true;
	}
}

bool is_good() {
	for(int i = 0; i < R; i++) {
		for(int j = 0; j < C; j++) {
			coords[i][j] = INF;
		}
	}

	for(int i = 0; i < R; i++) {
		for(int j = 0; j < C; j++) {
			if('a' <= grid[i][j] && grid[i][j] <= 'z') {
				if(!flood(pt(i, j), mp(1, 0), grid[i][j])) return false;
				vector<pair<int, int>> pts;
				for(int a = 0; a < R; a++) {
					for(int b = 0; b < C; b++) {
						if(grid[a][b] == '.') continue;
						pts.push_back(make_pair(coords[a][b].X, coords[a][b].Y));
					}
				}
				sort(pts.begin(), pts.end());
				return unique(pts.begin(), pts.end()) == pts.end();
			}
		}
	}
	assert(false);
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> R >> C;
	for(int i = 0; i < R; i++) {
		cin >> grid[i];
	}
	for(int i = 0; i < R; i++) {
		for(int j = 0; j < C; j++) {
			if(grid[i][j] == '0') {
				grid[i][j] = '0' + (J++);
			}
		}
	}

	int res = R * C;

	for(int m = 0; m < (1 << (2 * J)); m++) {
		for(int j = 0; j < J; j++) {
			ori[j] = un[3 & (m >> (2 * j))];
		}
		if(is_good()) {
			int minx = INF, miny = INF, maxx = -INF, maxy = -INF;
			for(int i = 0; i < R; i++) {
				for(int j = 0; j < C; j++) {
					if(grid[i][j] == '.') continue;
					minx = min(minx, coords[i][j].X);
					miny = min(miny, coords[i][j].Y);
					maxx = max(maxx, coords[i][j].X);
					maxy = max(maxy, coords[i][j].Y);
				}
			}
			res = min(res, (maxx - minx + 1) * (maxy - miny + 1));
		}
	}
	cout << res << '\n';

	return 0;
}
