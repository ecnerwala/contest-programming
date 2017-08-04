#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

typedef pair<ll, ll> pii;
#define X first
#define H second

const ll MAXN = 2e6;
ll N;

ll AX, BX, CX;
ll AH, BH, CH;
pii P[MAXN];

set<pii> poles;
typedef set<pii>::iterator iter;

inline ld sq(ld d) { return d * d; }

ld area(pii l, pii r) {
	assert(l.X < r.X);
	assert(abs(l.H - r.H) <= r.X - l.X);
	ld Hmid = (l.H + r.H) * 0.5 - (r.X - l.X) * 0.5;
	assert(Hmid <= l.H && Hmid <= r.H);
	return (sq(l.H) + sq(r.H)) * 0.5 - sq(Hmid);
}

ld area(pii a) {
	return sq(a.H) * 0.5;
}

ld go() {
	poles.clear();
	ld res = 0.0;
	ld cur = 0.0;
	for (ll i = 0; i < N; i++) {
		iter r = poles.lower_bound(P[i]);
		bool hasR = r != poles.end();
		bool hasL = r != poles.begin();
		iter l = r;
		if (hasL) l--;

		if (hasR) assert(r->X > P[i].X);
		if (hasL) assert(l->X < P[i].X);

		bool good = true;
		// Check if we're covered on either side
		if (hasR && r->H - P[i].H >= r->X - P[i].X) {
			good = false;
		}
		if (hasL && l->H - P[i].H >= P[i].X - l->X) {
			good = false;
		}

		if (good) {
			if (hasL && hasR) {
				cur -= area(*l, *r);
			}
			while (hasR && P[i].H - r->H >= r->X - P[i].X) {
				pii a = *r;
				r = poles.erase(r);
				hasR = r != poles.end();
				if (hasR) {
					cur -= area(a, *r);
				}
			}

			while (hasL && P[i].H - l->H >= P[i].X - l->X) {
				pii a = *l;
				l = poles.erase(l);
				hasL = l != poles.begin();
				if (hasL) l--;
				if (hasL) {
					cur -= area(*l, a);
				}
			}

			if (hasR) cur += area(P[i], *r);
			if (hasL) cur += area(*l, P[i]);

			poles.insert(P[i]);
		}

		assert(!poles.empty());
		ld val = cur + area(*poles.begin()) + area(*poles.rbegin());
		//cerr << P[i].X << ',' << P[i].H << ' ' << val << ' ' << poles.size() << '\n';
		res += val;
	}
	return res;
}

int main() {
	ll __T; cin >> __T;
	for (ll T = 1; T <= __T; T++) {
		cin >> N;
		cin >> P[0].X >> AX >> BX >> CX;
		cin >> P[0].H >> AH >> BH >> CH;
		for (ll i = 1; i < N; i++) {
			P[i].X = ll((AX * P[i-1].X + BX) % CX) + 1;
			P[i].H = ll((AH * P[i-1].H + BH) % CH) + 1;
			assert(1 <= P[i].X && P[i].X <= 2e7);
			assert(1 <= P[i].H && P[i].H <= 2e7);
		}

		ld res = go();
		cout << "Case #" << T << ": ";
		cout << fixed << setprecision(9) << res << '\n';
	}

	return 0;
}
