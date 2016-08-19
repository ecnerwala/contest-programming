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

bool is_before(pll a, pll b) {
	a.B *= -1, b.B *= -1;
	assert(a.A > 0);
	assert(b.A > 0);
	if(a.B / a.A < b.B / b.A) return true;
	if(a.B / a.A > b.B / b.A) return false;
	a.B %= a.A;
	b.B %= b.A;
	return (a.B * b.A < b.B * a.A);
}

// queries min along a hull, supports insert line and query point
// conditions: slopes decrease and points increase
struct hull_query {
	vector<pair<pll, int>> hull;
	int i;

	void insert(pll l, int ind) {
		assert(hull.empty() || hull.back().first.A > l.A);
		while(hull.size() >= 2) {
			pll a = hull[int(hull.size()) - 2].first;
			pll b = hull[int(hull.size()) - 1].first;
			assert(a.A > b.A && b.A > l.A);
			a = a - l, b = b - l;
			assert(a.A > 0);
			assert(b.A > 0);
			// if(-a.B / a.A < -b.B / b.A) break;
			//assert(is_before(a,b) == (-a.B * b.A < -b.B * a.A));
			if(is_before(a, b)) break;
			hull.pop_back();
		}
		hull.emplace_back(l, ind);
		i = min(i, int(hull.size()) - 1);
	}

	int query(ll v) {
		assert(!hull.empty());
		while(i+1 < int(hull.size()) &&
				eval(hull[i+1].first, v) <= eval(hull[i].first, v)) {
			i++;
		}
		return hull[i].second;
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
	int cnt[MAXN];

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

	pair<ll, int> update_hull(ll c = 0) {
		hull_query h;
		dp[0] = 0;
		cnt[0] = 0;
		for(int i = 0; i < N; i++) {
			h.insert(get_line(i), i);
			int p = h.query(P[i].B);
			dp[i+1] = min(get_cost(p, i+1) + c, INF);
			cnt[i+1] = cnt[p] + 1;
		}
		return pair<ll, int>(dp[N], cnt[N]);
	}

	ll go() {
		if(K == N) {
			update_hull(0);
			return dp[N];
		}
		// mi is >, ma is <=
		ll mi = 0, ma = INF;
		assert(update_hull(mi).second == N);
		assert(N > K);
		assert(update_hull(ma).second == 1);
		assert(1 <= K);
		while(ma - mi > 1) {
			ll md = (mi + ma) / 2;
			int c = update_hull(md).second;
			if(c > K) {
				mi = md;
			} else {
				ma = md;
			}
		}
		//cerr << mi << ' ' << ma << '\n';
		ll v1 = update_hull(mi).first - K * mi;
		ll v2 = update_hull(ma).first - K * ma;
		return max(v1, v2);
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
