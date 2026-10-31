#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e3;
const int MAXM = 1.1e3;
int N, M;
char G[MAXN][MAXM];
int cnt[MAXN][MAXM];

int getCnt(int xlo, int xhi, int ylo, int yhi) {
	xhi++, yhi++;
	return cnt[xhi][yhi] - cnt[xhi][ylo] - cnt[xlo][yhi] + cnt[xlo][ylo];
}

void fillRect(int xlo, int xhi, int ylo, int yhi) {
	if (xlo > xhi || ylo > yhi) return;

	assert(getCnt(xlo, xhi, ylo, yhi));

	assert(xlo <= xhi);
	assert(ylo <= yhi);
	if (xlo != xhi) {
		if (getCnt(xlo+1, xhi, ylo, yhi)) {
			fillRect(xlo+1, xhi, ylo, yhi);
			if (getCnt(xlo, xlo, ylo, yhi)) {
				fillRect(xlo, xlo, ylo, yhi);
			} else {
				for (int y = ylo; y <= yhi; y++) {
					G[xlo][y] = G[xlo+1][y] | '`';
				}
			}
		} else {
			assert(getCnt(xlo, xhi-1, ylo, yhi));
			fillRect(xlo, xhi-1, ylo, yhi);
			if (getCnt(xhi, xhi, ylo, yhi)) {
				fillRect(xhi, xhi, ylo, yhi);
			} else {
				for (int y = ylo; y <= yhi; y++) {
					G[xhi][y] = G[xhi-1][y] | '`';
				}
			}
		}
	} else if (ylo != yhi) {
		if (getCnt(xlo, xhi, ylo+1, yhi)) {
			fillRect(xlo, xhi, ylo+1, yhi);
			if (getCnt(xlo, xhi, ylo, ylo)) {
				fillRect(xlo, xhi, ylo, ylo);
			} else {
				for (int x = xlo; x <= xhi; x++) {
					G[x][ylo] = G[x][ylo+1] | '`';
				}
			}
		} else {
			assert(getCnt(xlo, xhi, ylo, yhi-1));
			fillRect(xlo, xhi, ylo, yhi-1);
			if (getCnt(xlo, xhi, yhi, yhi)) {
				fillRect(xlo, xhi, yhi, yhi);
			} else {
				for (int x = xlo; x <= xhi; x++) {
					G[x][yhi] = G[x][yhi-1] | '`';
				}
			}
		}
	} else {
		// do nothing
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> G[i];
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cnt[i+1][j+1] = cnt[i][j+1] + cnt[i+1][j] - cnt[i][j] + (G[i][j] != '.');
		}
	}

	vector<int> xcoords({-1, N});
	vector<int> ycoords({-1, M});
	int xA = -1, yA = -1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] != '.') {
				xcoords.push_back(i);
				ycoords.push_back(j);
				if (G[i][j] == 'A') {
					xA = i, yA = j;
				}
			}
		}
	}

	int bestArea = 0;
	int bxlo = -1, bxhi = -1, bylo = -1, byhi = -1;
	for (int xlo : xcoords) {
		if (xlo >= xA) continue;
		for (int xhi : xcoords) {
			if (xhi <= xA) continue;
			for (int ylo : ycoords) {
				if (ylo >= yA) continue;
				for (int yhi : ycoords) {
					if (yhi <= yA) continue;
					if (getCnt(xlo+1, xhi-1, ylo+1, yhi-1) > 1) continue;

					int area = (xhi - xlo - 1) * (yhi - ylo - 1);
					if (area > bestArea) {
						bestArea = area;
						bxlo = xlo, bxhi = xhi, bylo = ylo, byhi = yhi;
					}
				}
			}
		}
	}
	assert(bestArea);
	fillRect(bxlo+1, bxhi-1, bylo+1, byhi-1);
	fillRect(0, bxlo, bylo+1, byhi-1);
	fillRect(bxhi, N-1, bylo+1, byhi-1);
	fillRect(0, N-1, 0, bylo);
	fillRect(0, N-1, byhi, M-1);

	for (int i = 0; i < N; i++) {
		assert(G[i][M] == '\0');
		cout << G[i] << '\n';
	}

	return 0;
}
