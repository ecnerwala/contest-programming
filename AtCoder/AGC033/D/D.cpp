#include<bits/stdc++.h>
using namespace std;

void setmax(uint8_t& a, int v) {
	if (v > a) a = uint8_t(v);
}

const int MAXX = 190;
const int MAXY = 190;
const int MAXV = 18;
int X, Y;
char A[MAXX][MAXY];
int pref[MAXX][MAXY];
uint8_t dpx[MAXX][MAXY][MAXY][MAXV];
uint8_t dpy[MAXX][MAXY][MAXX][MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> X >> Y;
	for (int x = 0; x < X; x++) {
		cin >> A[x];
		//for (int y = 0; y < Y; y++) { A[x][y] = ".#"[(x+y)&1]; }
	}
	for (int x = 1; x <= X; x++) {
		for (int y = 1; y <= Y; y++) {
			pref[x][y] = pref[x-1][y] + pref[x][y-1] - pref[x-1][y-1] + (A[x-1][y-1] == '#');
		}
	}
	auto rectSum = [&](int xlo, int xhi, int ylo, int yhi) -> int {
		return pref[xhi][yhi] - pref[xlo][yhi] - pref[xhi][ylo] + pref[xlo][ylo];
	};

	auto isUniform = [&](int xlo, int xhi, int ylo, int yhi) -> int {
		int s = rectSum(xlo, xhi, ylo, yhi);
		return s == 0 || s == (xhi - xlo) * (yhi - ylo);
	};

	for (int xlo = X-1; xlo >= 0; xlo--) {
		for (int ylen = 1; ylen <= Y; ylen++) {
			for (int ylo = 0, yhi = ylen; yhi <= Y; ylo++, yhi++) {
				if (isUniform(xlo, xlo+1, ylo, yhi)) {
					if (xlo + 1 < X && isUniform(xlo, xlo+2, ylo, yhi)) {
						dpx[xlo][ylo][yhi][0] = dpx[xlo+1][ylo][yhi][0];
					} else {
						dpx[xlo][ylo][yhi][0] = uint8_t(xlo + 1);
					}
				} else {
					dpx[xlo][ylo][yhi][0] = uint8_t(xlo);
				}
			}
		}
	}
	for (int ylo = Y-1; ylo >= 0; ylo--) {
		for (int xlen = 1; xlen <= X; xlen++) {
			for (int xlo = 0, xhi = xlen; xhi <= X; xlo++, xhi++) {
				if (isUniform(xlo, xhi, ylo, ylo+1)) {
					if (ylo+1 < Y && isUniform(xlo, xhi, ylo, ylo+2)) {
						dpy[xlo][ylo][xhi][0] = dpy[xlo][ylo+1][xhi][0];
					} else {
						dpy[xlo][ylo][xhi][0] = uint8_t(ylo + 1);
					}
				} else {
					dpy[xlo][ylo][xhi][0] = uint8_t(ylo);
				}
			}
		}
	}
	if (isUniform(0, X, 0, Y)) {
		cout << 0 << '\n';
		exit(0);
	}
	for (int v = 1; v < MAXV; v++) {
		for (int xlo = 0; xlo < X; xlo++) {
			for (int ylo = 0; ylo < Y; ylo++) {
				for (int yhi = ylo+1; yhi <= Y; yhi++) {
					if (dpx[xlo][ylo][yhi][v-1] == X) {
						dpx[xlo][ylo][yhi][v] = uint8_t(X);
						continue;
					}
					dpx[xlo][ylo][yhi][v] = dpx[dpx[xlo][ylo][yhi][v-1]][ylo][yhi][v-1];
				}
				for (int xhi = xlo + 1; xhi <= X; xhi++) {
					if (dpy[xlo][ylo][xhi][v-1] == Y) {
						dpy[xlo][ylo][xhi][v] = uint8_t(Y);
						continue;
					}
					dpy[xlo][ylo][xhi][v] = dpy[xlo][dpy[xlo][ylo][xhi][v-1]][xhi][v-1];
				}

				for (int xhi = xlo + 1; xhi <= X; xhi++) {
					int yhi = dpy[xlo][ylo][xhi][v];
					setmax(dpx[xlo][ylo][yhi][v], xhi);
				}
				for (int yhi = ylo + 1; yhi <= Y; yhi++) {
					int xhi = dpx[xlo][ylo][yhi][v];
					setmax(dpy[xlo][ylo][xhi][v], yhi);
				}
				for (int xhi = X-1; xhi > xlo; xhi--) {
					setmax(dpy[xlo][ylo][xhi][v], dpy[xlo][ylo][xhi+1][v]);
				}
				for (int yhi = Y-1; yhi > ylo; yhi--) {
					setmax(dpx[xlo][ylo][yhi][v], dpx[xlo][ylo][yhi+1][v]);
				}
			}
		}
		if (dpx[0][0][Y][v] == X) {
			cout << v << '\n';
			exit(0);
		}
	}
	assert(false);

	return 0;
}
