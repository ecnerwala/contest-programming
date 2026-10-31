#include <bits/stdc++.h>

#include "geodeb.h"

using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x_=0, T y_=0) : x(x_), y(y_) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend std::ostream& operator<<(std::ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

template<class T> struct Point3D {
	typedef Point3D P;
	typedef const P& R;
	T x, y, z;
	explicit Point3D(T x_=0, T y_=0, T z_=0) : x(x_), y(y_), z(z_) {}
	bool operator<(R p) const {
		return tie(x, y, z) < tie(p.x, p.y, p.z); }
	bool operator==(R p) const {
		return tie(x, y, z) == tie(p.x, p.y, p.z); }
	P operator+(R p) const { return P(x+p.x, y+p.y, z+p.z); }
	P operator-(R p) const { return P(x-p.x, y-p.y, z-p.z); }
	P operator*(T d) const { return P(x*d, y*d, z*d); }
	P operator/(T d) const { return P(x/d, y/d, z/d); }
	T dot(R p) const { return x*p.x + y*p.y + z*p.z; }
	P cross(R p) const {
		return P(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
	}
	T dist2() const { return x*x + y*y + z*z; }
	double dist() const { return sqrt((double)dist2()); }
	//Azimuthal angle (longitude) to x-axis in interval [-pi, pi]
	double phi() const { return atan2(y, x); }
	//Zenith angle (latitude) to the z-axis in interval [0, pi]
	double theta() const { return atan2(sqrt(x*x+y*y),z); }
	P unit() const { return *this/(T)dist(); } //makes dist()=1
	//returns unit vector normal to *this and p
	P normal(P p) const { return cross(p).unit(); }
	//returns point rotated 'angle' radians ccw around axis
	P rotate(double angle, P axis) const {
		double s = sin(angle), c = cos(angle); P u = axis.unit();
		return u*dot(u)*(1-c) + (*this)*c - cross(u)*s;
	}
};
typedef Point3D<double> P3;

struct PR {
	void ins(int x) { (a == -1 ? a : b) = x; }
	void rem(int x) { (a == x ? a : b) = -1; }
	int cnt() { return (a != -1) + (b != -1); }
	int a, b;
};

struct F { P3 q; int a, b, c; };

std::vector<F> hull3d(const std::vector<P3>& A) {
	assert(sz(A) >= 4);
	std::vector<std::vector<PR>> E(sz(A), std::vector<PR>(sz(A), {-1, -1}));
#define E(x,y) E[f.x][f.y]
	std::vector<F> FS;
	auto mf = [&](int i, int j, int k, int l) {
		P3 q = (A[j] - A[i]).cross((A[k] - A[i]));
		if (q.dot(A[l]) > q.dot(A[i]))
			q = q * -1;
		F f{q, i, j, k};
		E(a,b).ins(k); E(a,c).ins(j); E(b,c).ins(i);
		FS.push_back(f);
	};
	rep(i,0,4) rep(j,i+1,4) rep(k,j+1,4)
		mf(i, j, k, 6 - i - j - k);

	rep(i,4,sz(A)) {
		rep(j,0,sz(FS)) {
			F f = FS[j];
			if(f.q.dot(A[i]) > f.q.dot(A[f.a])) {
				E(a,b).rem(f.c);
				E(a,c).rem(f.b);
				E(b,c).rem(f.a);
				swap(FS[j--], FS.back());
				FS.pop_back();
			}
		}
		int nw = sz(FS);
		rep(j,0,nw) {
			F f = FS[j];
#define C(a, b, c) if (E(a,b).cnt() != 2) mf(f.a, f.b, i, f.c);
			C(a, b, c); C(a, c, b); C(b, c, a);
		}
	}
	for (F& it : FS) if ((A[it.b] - A[it.a]).cross(
		A[it.c] - A[it.a]).dot(it.q) <= 0) swap(it.c, it.b);
	return FS;
};
template<class P, class F>
void delaunay(std::vector<P>& ps, F trifun) {
	if (sz(ps) == 3) { int d = (ps[0].cross(ps[1], ps[2]) < 0);
		trifun(0,1+d,2-d); }
	std::vector<P3> p3;
	for (P p : ps) p3.emplace_back(p.x, p.y, p.dist2());
	if (sz(ps) > 3) for(auto t:hull3d(p3)) if ((p3[t.b]-p3[t.a]).
			cross(p3[t.c]-p3[t.a]).dot(P3(0,0,1)) < 0)
		trifun(t.a, t.c, t.b);
}

typedef Point<double> P;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	GD_INIT("output.html");

	auto read_int = [&]() {
		int res = 0;
		while (true) {
			char c; cin >> c;
			if ('0' <= c && c <= '9') {
				res *= 10;
				res += c - '0';
			} else break;
		}
		return res;
	};

	const int N = 20;
	std::vector<P> ps(N);
	std::vector<string> names(N);
	for (int i = 0; i < N; i++) {
		int a, b, c; string s;
		a = read_int();
		b = read_int();
		c = read_int();
		cin >> s;
		assert(s == "N");
		ps[i].y = a + (b + c / 60.) / 60.;
		ps[i].y -= 37;

		a = read_int();
		b = read_int();
		c = read_int();
		cin >> s;
		assert(s == "W");
		ps[i].x = a + (b + c / 60.) / 60.;
		ps[i].x *= -1;
		ps[i].x -= -(109 + 3 / 60.);

		cin >> s;
		cerr << ps[i].x << '\t' << ps[i].y << '\t' << s << '\n';
		names[i] = s;
	}

	GD_RECT(0, 0, 109-102, 41-37);

	delaunay(ps, [&](int a, int b, int c) {
		GD_TRIANGLE(ps[a].x, ps[a].y, ps[b].x, ps[b].y, ps[c].x, ps[c].y);
	});
	for (int i = 0; i < N; i++) {
		cerr << ps[i].x * 100 << ' ' << 400 - ps[i].y * 100 << '\n';
	}
	for (int i = 0; i < N; i++) {
		GD_POINT(ps[i].x, ps[i].y) << names[i];
	}

	return 0;
}
