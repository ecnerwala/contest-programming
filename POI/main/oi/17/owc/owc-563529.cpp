#include<iostream>
#include<complex>
#include<cmath>
#include<cassert>

using namespace std;

typedef long long ll;
typedef complex<ll> pt;
#define X real()
#define Y imag()

inline ll cross(pt a, pt b) { return imag(conj(a) * b); }
inline ll dot(pt a, pt b) { return real(conj(a) * b); }

//ccw
inline ll area(pt a, pt b, pt c) {
	return cross(b - a, c - a);
}

inline bool ccw(pt a, pt b, pt c) {
	return area(a, b, c) > 0;
}

inline bool colinear(pt a, pt b, pt c) {
	return area(a, b, c) == 0;
}

const int MAXN = 1e3;
const int MAXK = 4e4;

ll MOD;

int N, K;

pt P[MAXN];
pt S[MAXK];

bool good[MAXN][MAXN];

int cnt[MAXN];

ll dp[MAXN][MAXN];

ll go() {
	for(int p = 0; p < N; p++) {
		for(int i = 0; i < N; i++) {
			cnt[i] = 0;
			good[p][i] = true;
		}
		for(int i = 0; i < K; i++) {
			pt s = S[i];
			int mi = p + 1; if(mi == N) mi = 0;
			int ma = p - 1; if(ma == -1) ma = N - 1;
			assert(ccw(P[p], P[mi], s) && !ccw(P[p], P[ma], s));

			while(ma != (mi + 1) % N) {
				int diff = ma - mi;
				if(diff < 0) diff += N;
				int md = mi + diff / 2;
				md %= N;
				if(ccw(P[p], P[md], s)) {
					mi = md;
				} else {
					ma = md;
				}
			}
			cnt[ma] ++;
			if(colinear(P[p], P[ma], s)) {
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

	for(int i = 0; i < N; i++) {
		good[i][i] = false;
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
	cin >> N >> K >> MOD;

	//screw this clockwise thing
	for(int i = N - 1; i >= 0; i--) {
		cin >> P[i].X >> P[i].Y;
	}
	
	for(int i = 0; i < K; i++) {
		cin >> S[i].X >> S[i].Y;
	}

	ll res = go();
	cout << res << '\n';
	return 0;
}
