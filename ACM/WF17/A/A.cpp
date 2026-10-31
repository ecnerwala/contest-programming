#include<bits/stdc++.h>
using namespace std;

#pragma GCC diagnostic ignored "-Wshadow"

template <class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point (T x=0, T y=0) : x(x), y(y) {}
	bool operator <(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator == (P p) const { return tie(x,y) == tie(p.x, p.y); }
	P operator+(P p) const { return P(x+p.x,y+p.y); }
	P operator-(P p) const { return P(x-p.x,y-p.y); }
	P operator*(T d) const { return P(x*d,y*d); }
	P operator/(T d) const { return P(x/d,y/d); }
	T dot(P p) const { return x*p.x+y*p.y; }
	T cross(P p) const { return x*p.y-y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x * x + y * y; }
	double dist() const { return sqrt((double)dist2()); }
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); }
	P perp() const { return P(-y,x); }
	P normal() const { return perp().unit(); }
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a));
	}
};

template <class P>
bool segmentIntersectionQ(P s1, P e1, P s2, P e2) {
	if (e1 == s1) {
		if (e2 == s2) return e1 == e2;
		swap(s1, s2); swap(e1, e2);
	}
	P v1 = e1-s1,v2=e2-s2, d = s2-s1;
	auto a = v1.cross(v2), a1=d.cross(v1),a2 = d.cross(v2);
	if (a==0) {
		auto b1 = s1.dot(v1), c1=e1.dot(v1), b2=s2.dot(v1), c2=e2.dot(v1);
		return !a1 && max(b1, min(b2,c2)) <= min(c1,max(b2,c2));
	}
	if (a < 0) { a = -a; a1 = -a1; a2 = -a2; }
	return (0 <= a1 && a1 <= a && 0 <= a2 && a2 <= a);
}

#pragma GCC diagnostic warning "-Wshadow"

template <class P>
int sameDir(P s, P t) {
	return s.cross(t) == 0 && s.dot(t) > 0;
}

template <class P>
bool isReflex(P s, P t) {
	auto c = s.cross(t);
	return c ? (c < 0) : (s.dot(t) < 0);
}

template <class P>
bool angleCmp(P base, P s, P t) {
	int r = isReflex(base, s) - isReflex(base, t);
	return r ? (r < 0) : (0 < s.cross(t));
}

// is x in [s,t], taken ccw, returns 1/0/-1 for in/boundary/out
template <class P>
int angleBetween(P s, P t, P x) {
	if (sameDir(x, s) || sameDir(x, t)) return 0;
	return angleCmp(s, x, t) ? 1 : -1;
}

using ll = long long;
using P = Point<ll>;

const int MAXN = 250;
int N;
P _pts[MAXN];
P *const pts = _pts + 1;

ll signum(ll a) { return (a > 0) - (a < 0); }

double maxRay(int i, P dir) {
	assert(0 <= i && i < N);
	double stop = 1e99;
	// vertices
	for (int j = 0; j < N; j++) {
		if (j == i || sameDir(pts[j] - pts[i], dir)) {
			if (angleBetween(pts[j+1]-pts[j], pts[j-1]-pts[j], dir) == -1) {
				// it's a stopping point
				stop = min(stop, (pts[j] - pts[i]).dist());
			}
		}
	}
	// edges
	for (int j = 0; j < N; j++) {
		P a = pts[j] - pts[i];
		P b = pts[j+1] - pts[i];
		auto ca = dir.cross(a), cb = dir.cross(b);
		// strictly above to strictly below
		if (signum(ca) * signum(cb) >= 0) continue;
		// must cross positive
		if (signum(a.cross(b)) != signum(cb)) continue;
		double cnd = (cb * double(dir.dot(a)) - ca * double(dir.dot(b))) / (cb - ca);
		cnd /= dir.dist();
		assert(cnd > 0);
		stop = min(stop, cnd);
	}
	return stop;
}
double maxLine(int i, P dir) {
	return maxRay(i, dir) + maxRay(i, P(0,0)-dir);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> pts[i].x >> pts[i].y;
	}
	pts[N] = pts[0];
	pts[-1] = pts[N-1];

	double res = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			res = max(res, maxLine(i, pts[j] - pts[i]));
		}
	}
	cout << fixed << setprecision(20) << res << '\n';

	return 0;
}
