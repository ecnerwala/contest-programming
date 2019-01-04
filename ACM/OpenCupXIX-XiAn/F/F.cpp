#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;

ll sq(ll a) { return a * a; }

ld go() {
	ll ox, oy, oz, r;
	cin >> ox >> oy >> oz >> r;
	ll sx, sy, sz;
	cin >> sx >> sy >> sz;
	ll tx, ty, tz;
	cin >> tx >> ty >> tz;

	sx -= ox, sy -= oy, sz -= oz;
	tx -= ox, ty -= oy, tz -= oz;

	// norms of the sides
	ll ns = sx * sx + sy * sy + sz * sz;
	ll nt = tx * tx + ty * ty + tz * tz;
	assert(ns >= r * r && nt >= r * r);
	ll nd = sq(sx - tx) + sq(sy - ty) + sq(sz - tz);

	if (nd <= abs(nt - ns)) {
		assert(min(ns, nt) >= r * r);
		// obtuse, so the answer is just the distance
		if (nd == 0) return 0;
		else return sqrt(ld(nd));
	}
	ll cross2 = sq(sx * ty - sy * tx) + sq(sy * tz - sz * ty) + sq(sz * tx - sx * tz);
	// square of the cross product
	if (cross2 >= nd * r * r) {
		// it's farther away
		if (nd == 0) return 0;
		else return sqrt(ld(nd));
	} else {
		// it's close
		ll dot = sx * tx + sy * ty + sz * tz;
		ld res;
		if (dot * dot == ns * nt) {
			assert(dot < 0);
			res = M_PI;
		} else {
			ld cosAng = ld(dot) / sqrt(ld(ns * nt));
			cosAng = min(cosAng, 1.l);
			cosAng = max(cosAng, -1.l);
			res = acos(cosAng);
		}
		assert(ns >= r * r);
		if (ns > r * r) {
			res -= acos(ld(r) / sqrt(ld(ns)));
		}
		assert(nt >= r * r);
		if (nt > r * r) {
			res -= acos(ld(r) / sqrt(ld(nt)));
		}
		res = max(res, 0.l); //should be true
		res *= r;
		if (ns > r * r) {
			res += sqrt(ld(ns - r * r));
		}
		if (nt > r * r) {
			res += sqrt(ld(nt - r * r));
		}
		return res;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	while (T--) {
		ld res = go();
		cout << fixed << setprecision(20) << res << '\n';
	}

	return 0;
}
