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


// queries min along a hull, supports insert line and query point
// conditions: slopes decrease and points increase
struct hull_query {
	vector<pll> hull;
	int i;

	void insert(pll l) {
		assert(hull.empty() || hull.back().A > l.A);
		while(hull.size() >= 2) {
			pll a = hull[int(hull.size()) - 2];
			pll b = hull[int(hull.size()) - 1];
			assert(a.A > b.A && b.A > l.A);
			a = a - l, b = b - l;
			assert(a.A > 0);
			assert(b.A > 0);
			// if(-a.B / a.A < -b.B / b.A) break;
			if(-a.B * b.A < -b.B * a.A) break;
			hull.pop_back();
		}
		hull.push_back(l);
		i = min(i, int(hull.size()) - 1);
	}

	ll query(ll v) {
		assert(!hull.empty());
		while(i+1 < int(hull.size()) &&
				eval(hull[i+1], v) <= eval(hull[i], v)) {
			i++;
		}
		return eval(hull[i], v);
	}

	hull_query() : hull(), i(0) { }
};

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

	ll get_cost(int l, int r) {
		assert(l < r);
		assert(r >= 1);
		return eval(get_line(l), P[r-1].B) + sq(P[r-1].B);
	}

	void update_hull() {
		hull_query h;
		for(int i = 0; i < N; i++) {
			h.insert(get_line(i));
			ll v = h.query(P[i].B) + sq(P[i].B);
			ndp[i+1] = v;
		}
	}

	ll go() {
		dp[0] = 0;
		for(int i = 1; i <= N; i++) dp[i] = INF;
		for(int r = 0; r < K; r++) {
			ndp[0] = 0;
			update_hull();
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
