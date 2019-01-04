#include<iostream>
#include<complex>
#include<cmath>
#include<cassert>

using namespace std;

typedef long long ll;

inline ll cross(ll x1, ll y1, ll x2, ll y2) {
	return x1 * y2 - y1 * x2;
}

//ccw
inline ll area(ll x1, ll y1, ll x2, ll y2, ll x3, ll y3) {
	return cross(x2 - x1, y2 - y1, x3 - x1, y3 - y1);
}

inline bool ccw(ll x1, ll y1, ll x2, ll y2, ll x3, ll y3) {
	return area(x1, y1, x2, y2, x3, y3) > 0;
}

inline bool colinear(ll x1, ll y1, ll x2, ll y2, ll x3, ll y3) {
	return area(x1, y1, x2, y2, x3, y3) == 0;
}

const int MAXN = 1e3;
const int MAXK = 4e4;

ll MOD;

int N, K;

ll PX[MAXN], PY[MAXN];
ll SX[MAXK], SY[MAXK];

bool good[MAXN][MAXN];

int cnt[MAXN];

ll dp[MAXN][MAXN];

ll DX[MAXN], DY[MAXN];

ll go() {
	for(int p = 0; p < N; p++) {
		for(int i = 0; i < N; i++) {
			cnt[i] = 0;
			good[p][i] = true;
		}
		ll px = PX[p], py = PY[p];
		for(int i = 0, j = p; i < N; i++, j++) {
			if(j == N) j = 0;
			DX[i] = PX[j] - px;
			DY[i] = PY[j] - py;
		}
		for(int i = 0; i < K; i++) {
			ll sx = SX[i] - px, sy = SY[i] - py;
			int mi = 1;
			int ma = N - 1;
			//assert(ccw(px, py, PX[mi], PY[mi], sx, sy) && !ccw(px, py, PX[ma], PY[ma], sx, sy));

			while(ma - mi > 1) {
				int diff = ma - mi;
				//if(diff == 1) break;
				int md = mi + diff / 2;
				md %= N;
				if(cross(DX[md], DY[md], sx, sy) > 0) {
					mi = md;
				} else {
					ma = md;
				}
			}
			cnt[ma] ++;
			if(cross(DX[ma], DY[ma], sx, sy) == 0) {
				int v = p + ma; if(v >= N) v -= N;
				good[p][v] = false;
			}
		}
		int tot = 0;
		//cerr << p << '\n';
		for(int i = 2, j = p + 2; i < N; i++, j++) {
			if(j >= N) j -= N;
			//cerr << cnt[i] << ' ';
			tot += cnt[i];
			if(tot % 2) {
				good[p][j] = false;
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
		for(int i = 0, j = d; i < N; i++, j++) {
			if(j == N) j = 0;
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
		//cin >> PX[i] >> PY[i];
	}
	
	for(int i = 0; i < K; i++) {
		cin >> SX[i] >> SY[i];
		//cin >> SX[i] >> SY[i];
	}

	ll res = go();
	cout << res << '\n';
	return 0;
}

