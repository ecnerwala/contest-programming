#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

namespace kactl {
bool Q;
struct Line {
	mutable ll k, m; mutable ld p;
	bool operator<(const Line& o) const {
		return Q ? p < o.p : k < o.k;
	}
};

const ld inf = 1/.0;
struct LineContainer : multiset<Line> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	ld div(ll a, ll b) { return ld(a)/ld(b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) { x->p = inf; return false; }
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ld query(ld x) {
		assert(!empty());
		Q = 1; auto l = *lower_bound({0,0,x}); Q = 0;
		return l.k * x + l.m;
	}
};
} // namespace kactl

using kactl::LineContainer;
using kactl::inf;

ld seg[1 << 20];

void go() {
	int N; cin >> N;
	//cerr << "START N " << N << '\n';
	LineContainer xlo, xhi, ylo, yhi;
	for (int i = 0; i < N; i++) {
		int x1, y1, x2, y2, vx, vy; cin >> x1 >> y1 >> x2 >> y2 >> vx >> vy;
		xlo.add(vx, x1);
		xhi.add(-vx, -x2);
		ylo.add(vy, y1);
		yhi.add(-vy, -y2);
	}
	//cerr << "xlo" << ' '; for (auto it : xlo) { cerr << it.k << ' ' << it.m << ' ' << it.p << '\n'; }
	//cerr << "xhi" << ' '; for (auto it : xhi) { cerr << it.k << ' ' << it.m << ' ' << it.p << '\n'; }
	//cerr << "ylo" << ' '; for (auto it : ylo) { cerr << it.k << ' ' << it.m << ' ' << it.p << '\n'; }
	//cerr << "yhi" << ' '; for (auto it : yhi) { cerr << it.k << ' ' << it.m << ' ' << it.p << '\n'; }

	vector<ld> timestamps;
	timestamps.push_back(0);
	timestamps.push_back(1e6);
	auto maybeInsert = [&](ld p) { if (0 < p && p < 1e6) timestamps.push_back(p); };
	for (auto it : xlo) { maybeInsert(it.p); }
	for (auto it : xhi) { maybeInsert(it.p); }
	for (auto it : ylo) { maybeInsert(it.p); }
	for (auto it : yhi) { maybeInsert(it.p); }
	sort(timestamps.begin(), timestamps.end());

	auto queryx = [&](ld t) { return -xhi.query(t) - xlo.query(t); };
	auto queryy = [&](ld t) { return -yhi.query(t) - ylo.query(t); };

	vector<pair<ld, ld>> pts;
	auto insertPt = [&](ld t) {
		ld x = queryx(t);
		ld y = queryy(t);
		if (x > 0 && y > 0) {
			pts.emplace_back(t, x * y);
		}
	};
	for (ld t : timestamps) insertPt(t);
	for (int i = 0; i + 1 < int(timestamps.size()); i++) {
		ld t0 = timestamps[i];
		ld t1 = timestamps[i+1];
		ld x0 = queryx(t0);
		ld x1 = queryx(t1);
		ld y0 = queryy(t0);
		ld y1 = queryy(t1);
		ld d0 = x0 * (y1 - y0) + y0 * (x1 - x0);
		ld d1 = x1 * (y1 - y0) + y1 * (x1 - x0);
		if ((d0 < 0 && d1 > 0) || (d0 > 0 && d1 < 0)) {
			insertPt(t0 - d0 / (d1 - d0) * (t1 - t0));
		}
	}

	sort(pts.begin(), pts.end());
	int S = 1;
	while (S <= int(pts.size())) S *= 2;
	for (int i = 1; i < 2 * S; i++) seg[i] = 0;
	for (int i = 0; i < int(pts.size()); i++) {
		seg[S+i] = pts[i].second;
	}
	for (int i = S-1; i > 0; i--) seg[i] = max(seg[2*i], seg[2*i+1]);

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		ld l, r; cin >> l >> r;
		ld lv = max(0.l, queryx(l)) * max(0.l, queryy(l));
		ld rv = max(0.l, queryx(r)) * max(0.l, queryy(r));
		ld ans = max(lv, rv);
		int li = int(lower_bound(pts.begin(), pts.end(), pair<ld, ld>{l, -1}) - pts.begin());
		int ri = int(lower_bound(pts.begin(), pts.end(), pair<ld, ld>{r, -1}) - pts.begin());
		for (int a = S+li, b = S+ri; a < b; a /= 2, b /= 2) {
			if (a & 1) {
				ans = max(ans, seg[a]);
				a++;
			}
			if (b & 1) {
				--b;
				ans = max(ans, seg[b]);
			}
		}
		cout << fixed << setprecision(12) << ans << '\n';
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Z; cin >> Z;
	while (Z--) {
		go();
	}

	return 0;
}
