#include <bits/stdc++.h>
using namespace std;

template <typename T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U> explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }

	friend std::ostream& operator << (std::ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
	friend std::istream& operator >> (std::istream& i, Point& p) { return i >> p.x >> p.y; }
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point operator + () const { return Point(+x, +y); }
	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	T dist2() const { return x * x + y * y; }
	friend auto norm(const Point& a) { return a.dist2(); }
	auto dist() const { return std::sqrt(dist2()); }
	friend auto abs(const Point& a) { return a.dist(); }
	Point unit() const { return *this / this->dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	auto angle() const { return std::atan2(y, x); }
	friend auto arg(const Point& a) { return a.angle(); }

	T int_norm() const { return __gcd(x,y); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.norm(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool same_dir(const Point& a, const Point& b) { return cross(a,b) == 0 && dot(a,b) > 0; }

	// check if 180 <= s..t < 360
	friend bool is_reflex(const Point& a, const Point& b) { auto c = cross(a,b); return c ? (c < 0) : (dot(a, b) < 0); }

	// operator < (s,t) for angles in [base,base+2pi)
	friend bool angle_less(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less(base, s, t); };
	}
	friend auto angle_cmp_center(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less(s, p, t) ? 1 : -1;
	}
};

template<class P>
vector<P> circleLine(P c, double r, P a, P b) {
	P ab = b - a, p = a + ab * dot(c-a,ab) / ab.dist2();
	double s = cross3(a, b, c), h2 = r*r - s*s / ab.dist2();
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
	vector<P> pts(2*N+1);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].x >> pts[i].y;
	}

	for (int i = N; i <= 2*N; i++) {
		pts[i] = pts[i-N];
	}

	for (int i = 0; i < N; i++) {
		L -= (pts[i+1] - pts[i]).dist();
	}

	assert(L > 0);

	int lo = 0;
	int hi = 1;
	L += (pts[hi] - pts[lo]).dist();
	P cur_st;

	double ans = 0;
	while (lo <= N) {
		P center = (pts[lo] + pts[hi]) / 2;
		P dir = pts[lo] - pts[hi];
		auto rot = dir.perp_ccw().unit();
		double major = L / 2;
		double minor = sqrt(major * major - dir.dist2() / 4);
		auto transform_pt = [&](P p) -> P {
			p = p - center;
			p = p.unrotate(rot);
			p.x /= minor;
			p.y /= major;
			return p;
		};
		auto untransform_pt = [&](P p) -> P {
			p.y *= major;
			p.x *= minor;
			p = p.rotate(rot);
			p = p + center;
			return p;
		};
		auto untransform_area = [&](double d) -> double {
			return d * minor * major;
		};

		P plo = transform_pt(pts[lo]);
		P plo1 = transform_pt(pts[lo+1]);
		P phi = transform_pt(pts[hi]);
		P phi1 = transform_pt(pts[hi+1]);

		P cndlo = circleLine(P(0,0), 1, plo, plo1)[1];
		P cndhi = circleLine(P(0,0), 1, phi, phi1)[0];
		double alo = cndlo.angle();
		double ahi = cndhi.angle();

		bool do_lo = alo < ahi;
		P nxt_st = untransform_pt(do_lo ? cndlo : cndhi);
		if (lo > 0) {
			P st = transform_pt(cur_st);
			double ast = st.angle();
			double cur_val = untransform_area(((do_lo ? alo : ahi) - ast) / 2);

			// y dx from a to b
			auto seg_val = [&](P a, P b) -> double {
				return (b.x - a.x) * (b.y + a.y) / 2;
			};

			cur_val += seg_val(nxt_st, center) + seg_val(center, cur_st);
			ans += cur_val;
		}
		cur_st = nxt_st;

		if (do_lo) {
			L -= (pts[lo+1] - pts[lo]).dist();
			lo++;
		} else {
			L += (pts[hi+1] - pts[hi]).dist();
			hi++;
		}
	}

	cout << fixed << setprecision(15) << ans << '\n';

	return 0;
}
