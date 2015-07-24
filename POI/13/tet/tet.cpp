#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1020;
const int S = 1 << 10;
const int MAXS = 2 * S;

inline void setmax(int &a, int b) { if(b > a) a = b; }

int N, M, Q;
// 0 is point update, range query
// 1 is range update, point query
int val[2][MAXS][2][MAXS];

// [a,b] by [c,d]
int a, b, c, d, h;

void queryy(int v[2][MAXS]) {
	// range query
	for(int y1 = c + S, y2 = d + S; y1 <= y2; y1 /= 2, y2 /= 2) {
		if(y1 % 2 == 1) {
			setmax(h, v[0][y1]);
			y1 ++;
		}
		if(y2 % 2 == 0) {
			setmax(h, v[0][y2]);
			y2--;
		}
	}
	// point query
	for(int y1 = c + S, y2 = d + S; y1; y1 /= 2, y2 /= 2) {
		setmax(h, v[1][y1]);
		setmax(h, v[1][y2]);
	}
}

void queryx() {
	// range query
	for(int x1 = a + S, x2 = b + S; x1 <= x2; x1 /= 2, x2 /= 2) {
		if(x1 % 2 == 1) {
			queryy(val[0][x1]);
			x1++;
		}
		if(x2 % 2 == 0) {
			queryy(val[0][x2]);
			x2--;
		}
	}
	// point query
	for(int x1 = a + S, x2 = b + S; x1; x1 /= 2, x2 /= 2) {
		queryy(val[1][x1]);
		queryy(val[1][x2]);
	}
}

void updatey(int v[2][MAXS]) {
	// range update
	for(int y1 = c + S, y2 = d + S; y1 <= y2; y1 /= 2, y2 /= 2) {
		if(y1 % 2 == 1) {
			setmax(v[1][y1], h);
			y1 ++;
		}
		if(y2 % 2 == 0) {
			setmax(v[1][y2], h);
			y2--;
		}
	}
	// point update
	for(int y1 = c + S, y2 = d + S; y1; y1 /= 2, y2 /= 2) {
		setmax(v[0][y1], h);
		setmax(v[0][y2], h);
	}
}

void updatex() {
	// range update
	for(int x1 = a + S, x2 = b + S; x1 <= x2; x1 /= 2, x2 /= 2) {
		if(x1 % 2 == 1) {
			updatey(val[1][x1]);
			x1++;
		}
		if(x2 % 2 == 0) {
			updatey(val[1][x2]);
			x2--;
		}
	}
	// point update
	for(int x1 = a + S, x2 = b + S; x1; x1 /= 2, x2 /= 2) {
		updatey(val[0][x1]);
		updatey(val[0][x2]);
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> Q;
	int res = 0;
	for(int i = 0; i < Q; i++) {
		int n, m, v, x, y;
		cin >> n >> m >> v >> x >> y;
		a = x, b = x + n - 1, c = y, d = y + m - 1;
		assert(0 <= a && a <= b && b < N);
		assert(0 <= c && c <= d && d < M);
		h = 0;
		queryx();
		h += v;
		updatex();
		res = max(res, h);
	}
	cout << res << '\n';
	return 0;
}
