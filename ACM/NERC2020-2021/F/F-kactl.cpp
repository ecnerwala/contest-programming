#include <bits/stdc++.h>
using namespace std;

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
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};
template<class P>
pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
	auto d = (e1 - s1).cross(e2 - s2);
	if (d == 0) // if parallel
		return {-(s1.cross(e1, s2) == 0), P(0, 0)};
	auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
	return {1, (s1 * p + e1 * q) / d};
}
template<class P>
vector<P> circleLine(P c, double r, P a, P b) {
	P ab = b - a, p = a + ab * (c-a).dot(ab) / ab.dist2();
	double s = a.cross(b, c), h2 = r*r - s*s / ab.dist2();
	if (h2 < 0) return {};
	if (h2 == 0) return {p};
	P h = ab.unit() * sqrt(h2);
	return {p - h, p + h};
}

using P = Point<double>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; double L; cin >> N >> L;
	vector<P> pts(3*N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].x >> pts[i].y;
	}

	{
		P center = pts[0];
		double ang = (pts[0] - pts[N-1]).angle();
		for (int i = 0; i < N; i++) {
			pts[i] = pts[i] - center;
			pts[i] = pts[i].rotate(-ang);
		}
	}

	for (int i = 0; i < N; i++) {
		pts[2*N+i] = pts[N+i] = pts[i];
	}

	for (int i = 0; i < N; i++) {
		L -= (pts[i+1] - pts[i]).dist();
	}

	assert(L > 0);
	//for (int i = 0; i < N; i++) { cerr << i << ' ' << pts[i] << '\n'; }

	int lo = 0;
	int hi = 1;
	P cur_st;
	while (true) {
		L += (pts[hi] - pts[hi-1]).dist();

		double mi = 0;
		double ma = L;
		for (int z = 0; z < 50; z++) {
			double md = (mi + ma) / 2;
			if (md + (pts[hi] - P(md, 0)).dist() <= L) {
				mi = md;
			} else {
				ma = md;
			}
		}
		cur_st = P(mi, 0);

		if (cur_st.cross(pts[hi], pts[hi+1]) >= 0) {
			break;
		} else {
			hi++;
			continue;
		}
	}
	//cerr << "cur_st" << cur_st << '\n';

	// y dx from a to b
	auto seg_val = [&](P a, P b) -> double {
		return (b.x - a.x) * (b.y + a.y) / 2;
	};

	double ans = 0;
	while (lo != N) {
		//cerr << lo << ' ' << hi << ' ' << cur_st << '\n';
		P center = (pts[lo] + pts[hi]) / 2;
		P dir = pts[lo] - pts[hi];
		double ang = dir.angle();
		double major = L / 2;
		double minor = sqrt(major * major - dir.dist2() / 4);
		auto transform_pt = [&](P p) -> P {
			p = p - center;
			p = p.rotate(-ang);
			p.x /= major;
			p.y /= minor;
			return p;
		};
		auto untransform_pt = [&](P p) -> P {
			p.y *= minor;
			p.x *= major;
			p = p.rotate(ang);
			p = p + center;
			return p;
		};
		auto untransform_area = [&](double d) -> double {
			return d * minor * major;
		};

		auto get_circ = [&](P a, P b) -> P {
			vector<P> res = circleLine(P(0,0), 1, a, b);
			assert(!res.empty());
			if (res.size() >= 2 && res[1].y > res[0].y) {
				swap(res[0], res[1]);
			}
			return res[0];
		};

		P plo = transform_pt(pts[lo]);
		P plo1 = transform_pt(pts[lo+1]);
		P phi = transform_pt(pts[hi]);
		P phi1 = transform_pt(pts[hi+1]);
		P st = transform_pt(cur_st);

		P cndlo = get_circ(plo, plo1);
		P cndhi = get_circ(phi, phi1);
		double alo = (cndlo*-1).perp().angle();
		double ahi = (cndhi*-1).perp().angle();
		double ast = (st*-1).perp().angle();

		bool do_lo = hi+1 == lo+N || (lo+1 < hi && alo < ahi);
		P nxt_st = untransform_pt(do_lo ? cndlo : cndhi);
		double cur_val = untransform_area(((do_lo ? alo : ahi) - ast) / 2);
		cur_val += seg_val(nxt_st, center) + seg_val(center, cur_st);

		if (do_lo) {
			L -= (pts[lo+1] - pts[lo]).dist();
			lo++;
		} else {
			L += (pts[hi+1] - pts[hi]).dist();
			hi++;
		}
		cur_st = nxt_st;

		//cerr << lo << ' ' << hi << ' ' << cur_st << '\n';

		ans += cur_val;
	}

	cout << fixed << setprecision(15) << ans << '\n';

	return 0;
}
