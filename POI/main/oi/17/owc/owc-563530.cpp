#include<iostream>
#include<complex>
#include<cmath>
#include<cassert>

using namespace std;

inline int cross(int x1, int y1, int x2, int y2) {
	return x1 * y2 - y1 * x2;
}

//ccw
inline int area(int x1, int y1, int x2, int y2, int x3, int y3) {
	return cross(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
}

inline bool ccw(int x1, int y1, int x2, int y2, int x3, int y3) {
	return area(x1, y1, x2, y2, x3, y3) > 0;
}

inline bool colinear(int x1, int y1, int x2, int y2, int x3, int y3) {
	return area(x1, y1, x2, y2, x3, y3) == 0;
}

const int MAXN = 1e3;
const int MAXK = 4e4;

int MOD;

int N, K;

int PX[MAXN], PY[MAXN];
int SX[MAXN], SY[MAXN];

bool good[MAXN][MAXN];

int cnt[MAXN];

int dp[MAXN][MAXN];

int go() {
	for(int p = 0; p < N; p++) {
		for(int i = 0; i < N; i++) {
			cnt[i] = 0;
			good[p][i] = true;
		}
		int px = PX[p], py = PY[p];
		for(int i = 0; i < K; i++) {
			int sx = SX[i], sy = SY[i];
			int mi = p + 1; if(mi == N) mi = 0;
			int ma = p - 1; if(ma == -1) ma = N - 1;
			//assert(ccw(P[p], P[mi], s) && !ccw(P[p], P[ma], s));

			while(ma != (mi + 1) % N) {
				int diff = ma - mi;
				if(diff < 0) diff += N;
				int md = mi + diff / 2;
				md %= N;
				if(ccw(px, py, PX[md], PY[md], sx, sy)) {
					mi = md;
				} else {
					ma = md;
				}
			}
			cnt[ma] ++;
			if(colinear(px, py, PX[ma], PY[ma], sx, sy)) {
				good[p][ma] = false;
			}
		}
		int tot = 0;
		//cerr << p << '\n';
		for(int i = (p + 2) % N; i != p; i++, i %= N) {
			//cerr << cnt[i] << ' ';
			tot += cnt[i];
			if(tot % 2) {
				good[p][i] = false;
			}
		}
		//cerr << '\n';
	}

	//for(int i = 0; i < N; i++) {
		//for(int j = 0; j < N; j++) {
			//cerr << good[i][j] << ' ';
		//}
		//cerr << '\n';
	//}

	for(int i = 0; i < N; i++) {
		dp[i][(i + 1) % N] = 1;
	}

	for(int d = 2; d < N; d++) {
		for(int i = 0; i < N; i++) {
			int j = (i + d) % N;
			dp[i][j] = 0;
			if(good[i][j]) {
				for(int k = (i + 1) % N; k != j; k++, k %= N) {
					dp[i][j] += dp[i][k] * dp[k][j];
					dp[i][j] %= MOD;
				}
			}
		}
	}

	return dp[1][0];
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> K >> MOD;

	//screw this clockwise thing
	for(int i = N - 1; i >= 0; i--) {
		cin >> PX[i] >> PY[i];
	}
	
	for(int i = 0; i < K; i++) {
		cin >> SX[i] >> SY[i];
	}

	int res = go();
	cout << res << '\n';
	return 0;
}
