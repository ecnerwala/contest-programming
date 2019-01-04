#include<bits/stdc++.h>
using namespace std;


const int MAXR = 40;
const int MAXC = 40;
int R, C;
char G[MAXR][MAXC];

bool good[MAXR][MAXC];

int flood(int x, int y) {
	if (!(0 <= x && x < R && 0 <= y && y < C)) {
		return 0;
	}
	if (!good[x][y]) {
		return 0;
	}
	good[x][y] = false;
	int res = 1;
	res += flood(x-1, y);
	res += flood(x+1, y);
	res += flood(x, y-1);
	res += flood(x, y+1);
	return res;
}

int max_good() {
	int res = 0;
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			res = max(res, flood(i, j));
		}
	}
	return res;
}

int go() {
	int res = 0;
	for (int z = 0; z < 2; z++) {
		char c = z ? 'W' : 'B';
		{ // just c
			bool has = false;
			for (int i = 0; i < R; i++) {
				for (int j = 0; j < C; j++) {
					if (G[i][j] == c) {
						has = true;
					}
				}
			}
			if (has) {
				for (int i = 0; i < R; i++) {
					for (int j = 0; j < C; j++) {
						good[i][j] = (G[i][j] == c);
					}
				}
				res = max(res, max_good());
			}
		}

		{ // vertical c//!c
			bool has = false;
			for (int i = 0; i + 1 < R; i++) {
				for (int j = 0; j < C; j++) {
					if (G[i][j] == c && G[i+1][j] != c) {
						has = true;
					}
				}
			}
			if (has) {
				for (int x = 0; x + 1 < R; x++) {
					for (int i = 0; i < R; i++) {
						for (int j = 0; j < C; j++) {
							if (i <= x) {
								good[i][j] = (G[i][j] == c);
							} else {
								good[i][j] = (G[i][j] != c);
							}
						}
					}
					res = max(res, max_good());
				}
			}
		}

		{ // horizontal c // !c
			bool has = false;
			for (int i = 0; i < R; i++) {
				for (int j = 0; j + 1 < C; j++) {
					if (G[i][j] == c && G[i][j+1] != c) {
						has = true;
					}
				}
			}

			if (has) {
				for (int y = 0; y + 1 < C; y++) {
					for (int i = 0; i < R; i++) {
						for (int j = 0; j < C; j++) {
							if (j <= y) {
								good[i][j] = (G[i][j] == c);
							} else {
								good[i][j] = (G[i][j] != c);
							}
						}
					}
					res = max(res, max_good());
				}
			}
		}

		{ // one of four
			for (int dx = 0; dx <= 1; dx++) {
				for (int dy = 0; dy <= 1; dy++) {
					bool has = false;
					for (int i = 0; i + 1 < R; i++) {
						for (int j = 0; j + 1 < C; j++) {
							if (
									G[i+dx][j+dy] == c &&
									G[i+dx][j+!dy] != c &&
									G[i+!dx][j+dy] != c &&
									G[i+!dx][j+!dy] != c
							) {
								has = true;
							}
						}
					}
					if (has) {
						for (int x = 0; x + 1 < R; x++) {
							for (int y = 0; y + 1 < C; y++) {
								for (int i = 0; i < R; i++) {
									for (int j = 0; j < C; j++) {
										if (dx == (i > x) && dy == (j > y)) {
											good[i][j] = (G[i][j] == c);
										} else {
											good[i][j] = (G[i][j] != c);
										}
									}
								}
								res = max(res, max_good());
							}
						}
					}
				}
			}
		}

		{ // diagonals
			bool has = false;
			for (int i = 0; i + 1 < R; i++) {
				for (int j = 0; j + 1 < C; j++) {
					if (G[i][j] == c && G[i+1][j] != c && G[i][j+1] != c && G[i+1][j+1] == c) {
						has = true;
					}
				}
			}
			if (has) {
				for (int x = 0; x + 1 < R; x++) {
					for (int y = 0; y + 1 < C; y++) {
						for (int i = 0; i < R; i++) {
							for (int j = 0; j < C; j++) {
								if ((i > x) == (j > y)) {
									good[i][j] = G[i][j] == c;
								} else {
									good[i][j] = G[i][j] != c;
								}
							}
						}
						res = max(res, max_good());
					}
				}
			}

		}

	}

	return res;
}

int main() {
	int T; scanf("%d", &T);

	for (int case_num = 1; case_num <= T; case_num ++) {
		scanf("%d %d", &R, &C);
		for (int i = 0; i < R; i++) {
			scanf(" %s", G[i]);
		}

		int res = go();
		printf("Case #%d: %d\n", case_num, res);
	}

	return 0;
}
