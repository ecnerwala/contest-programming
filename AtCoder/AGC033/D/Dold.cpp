#include<bits/stdc++.h>
using namespace std;

const int MAXX = 190;
const int MAXY = 190;
const int MAXV = 18;
int X, Y;
char A[MAXX][MAXY];
int pref[MAXX][MAXY];
uint8_t dp[MAXX][MAXY][MAXY][MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> X >> Y;
	for (int x = 0; x < X; x++) {
		//cin >> A[x];
		for (int y = 0; y < Y; y++) { A[x][y] = ".#"[(x+y)&1]; }
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
						dp[xlo][ylo][yhi][0] = dp[xlo+1][ylo][yhi][0];
					} else {
						dp[xlo][ylo][yhi][0] = uint8_t(xlo + 1);
					}
				} else {
					dp[xlo][ylo][yhi][0] = uint8_t(xlo);
				}

				for (int ymid = ylo + 1; ymid < yhi; ymid++) {
					for (int v = 1; v < MAXV; v++) {
						dp[xlo][ylo][yhi][v] = max(dp[xlo][ylo][yhi][v], min(dp[xlo][ylo][ymid][v-1], dp[xlo][yhi][ymid][v-1]));
					}
				}
				for (int v = 1; v < MAXV; v++) {
					if (dp[xlo][ylo][yhi][v-1] == X) {
						dp[xlo][ylo][yhi][v] = uint8_t(X);
						continue;
					}
					dp[xlo][ylo][yhi][v] = max(dp[xlo][ylo][yhi][v], dp[dp[xlo][ylo][yhi][v-1]][ylo][yhi][v-1]);
				}
				for (int v = 0; v < MAXV; v++) {
					dp[xlo][yhi][ylo][v] = dp[xlo][ylo][yhi][v];
				}
				//for (int v = 0; v < MAXV; v++) { cerr << xlo << ' ' << ylo << ' ' << yhi << ' ' << v << ' ' << int(dp[xlo][ylo][yhi][v]) << '\n'; }
			}
		}
	}
	for (int v = 0; v < MAXV; v++) {
		if (dp[0][0][Y][v] == X) {
			cout << v << '\n';
			exit(0);
		}
	}
	assert(false);

	return 0;
}
