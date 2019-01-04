#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1020;
const int S = 1 << 10;
const int MAXS = 2 * S + 20;

int N, M, Q;
int val[MAXS][MAXS];
int lzy[MAXS][MAXS];

// [a,b] by [c,d]
int query(int a, int b, int c, int d) {
	int res = 0;
	for(int x1 = a + S, x2 = b + S; x1 <= x2; x1 /= 2, x2 /= 2) {
		if(x1 % 2 == 1) {
			for(int y1 = c + S, y2 = d + S; y1 <= y2; y1 /= 2, y2 /= 2) {
				if(y1 % 2 == 1) {
					res = max(res, val[x1][y1]);
					y1 ++;
				}
				if(y2 % 2 == 0) {
					res = max(res, val[x1][y2]);
					y2--;
				}
			}
			x1++;
		}
		if(x2 % 2 == 0) {
			for(int y1 = c + S, y2 = d + S; y1 <= y2; y1 /= 2, y2 /= 2) {
				if(y1 % 2 == 1) {
					res = max(res, val[x2][y1]);
					y1 ++;
				}
				if(y2 % 2 == 0) {
					res = max(res, val[x2][y2]);
					y2--;
				}
			}
			x2--;
		}
	}
	for(int x1 = a + S, x2 = b + S; x1; x1 /= 2, x2 /= 2) {
		for(int y1 = c + S, y2 = d + S; y1; y1 /= 2, y2 /= 2) {
			res = max(res, lzy[x1][y1]);
			res = max(res, lzy[x1][y2]);
			res = max(res, lzy[x2][y1]);
			res = max(res, lzy[x2][y2]);
		}
	}
	return res;
}

void update(int a, int b, int c, int d, int v) {
	for(int x1 = a + S, x2 = b + S; x1 <= x2; x1 /= 2, x2 /= 2) {
		if(x1 % 2 == 1) {
			for(int y1 = c + S, y2 = d + S; y1 <= y2; y1 /= 2, y2 /= 2) {
				if(y1 % 2 == 1) {
					lzy[x1][y1] = max(lzy[x1][y1], v);
					y1 ++;
				}
				if(y2 % 2 == 0) {
					lzy[x1][y2] = max(lzy[x1][y2], v);
					y2--;
				}
			}
			x1++;
		}
		if(x2 % 2 == 0) {
			for(int y1 = c + S, y2 = d + S; y1 <= y2; y1 /= 2, y2 /= 2) {
				if(y1 % 2 == 1) {
					lzy[x2][y1] = max(lzy[x2][y1], v);
					y1 ++;
				}
				if(y2 % 2 == 0) {
					lzy[x2][y2] = max(lzy[x2][y2], v);
					y2--;
				}
			}
			x2--;
		}
	}
	for(int x1 = a + S, x2 = b + S; x1; x1 /= 2, x2 /= 2) {
		for(int y1 = c + S, y2 = d + S; y1; y1 /= 2, y2 /= 2) {
			val[x1][y1] = max(val[x1][y1], v);
			val[x1][y2] = max(val[x1][y2], v);
			val[x2][y1] = max(val[x2][y1], v);
			val[x2][y2] = max(val[x2][y2], v);
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> Q;
	int res = 0;
	for(int i = 0; i < Q; i++) {
		int n, m, h, x, y;
		cin >> n >> m >> h >> x >> y;
		h += query(x, x + n - 1, y, y + m - 1);
		update(x, x + n - 1, y, y + m - 1, h);
		res = max(res, h);
	}
	cout << res << '\n';
	return 0;
}
