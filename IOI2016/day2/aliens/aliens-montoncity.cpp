#include "aliens.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define A first
#define B second
// lines are stored as A = slope, B = const (y = l.A * x + l.B)

static const ll INF = 4e18;

inline ll sq(ll a) { return a * a; }
inline ll eval(pll l, ll x) { return l.A * x + l.B; }
inline pll operator - (pll a, pll b) {
	return pll(a.A - b.A, a.B - b.B);
}


struct photos {

	static const int MAXN = 1.5e5;
	static const int MAXM = 2e6;
	static const int MAXK = MAXN;

	int N, M, K;
	pll P[MAXN];

	// cost of adding a new one is
	// (r_l - c_r)^2 - (max(c_(l-1) - r_l, 0))^2
	// (-max(c_(l-1) - r_l, 0)^2 + r_l^2) - 2 r_l c_r + c_r^2

	ll dp[MAXN];
	ll ndp[MAXN];

	pll get_line(ll l) {
		ll slope = -2 * P[l].A;
		ll val = sq(P[l].A);
		if(l > 0 && P[l-1].B > P[l].A) {
			val -= sq(P[l-1].B - P[l].A);
		}
		val += dp[l];
		return pll(slope, val);
	}

	ll get_cost(pll l, int r) {
		assert(r >= 1);
		return eval(l, P[r-1].B) + sq(P[r-1].B);
	}

	// [l,r], checking [mi, ma]
	void update(int l, int r, int mi, int ma) {
		if(r < l) return;
		assert(mi <= ma);
		assert(mi < l);
		int i = (l + r) / 2;
		int md = -1;
		ll best = INF;
		for(int j = mi; j < i && j <= ma; j++) {
			ll cnd = get_cost(get_line(j), i);
			if(cnd < best) {
				best = cnd;
				md = j;
			}
		}
		assert(md != -1);
		ndp[i] = best;
		update(l, i-1, mi, md);
		update(i+1, r, md, ma);
	}

	ll go() {
		dp[0] = 0;
		for(int i = 1; i <= N; i++) dp[i] = INF;
		for(int r = 0; r < K; r++) {
			update(1, N, 0, N);
			memcpy(dp, ndp, sizeof(ndp));
		}
		return dp[N];
	}

	photos(int n, int m, int k, vector<int> r, vector<int> c) : N(n), M(m), K(k) {
		for(int i = 0; i < N; i++) P[i] = pll(min(r[i], c[i]), max(r[i], c[i]) + 1);

		for(int i = 0; i < N; i++) assert(P[i].A < P[i].B);

		for(int i = 0; i < N; i++) P[i].B *= -1;
		sort(P, P + N);
		for(int i = 0; i < N; i++) P[i].B *= -1;

		int l = 0;
		for(int i = 1; i < N; i++) {
			if(P[i].second > P[l].second) {
				l++;
				P[l] = P[i];
			}
		}
		N = l+1;
		K = min(K, N);

		for(int i = 1; i < N; i++) {
			assert(P[i-1].A < P[i].A);
			assert(P[i-1].B < P[i].B);
		}

		cerr << N << '\n';
		for(int i = 0; i < N; i++) cerr << P[i].A << ' ' << P[i].B << '\n';
	}
};

long long take_photos(int n, int m, int k, std::vector<int> r, std::vector<int> c) {
	return photos(n, m, k, r, c).go();
}
