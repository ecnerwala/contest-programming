#include <bits/stdc++.h>

template <typename T, typename AreaT=T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U, typename V> explicit Point(const Point<U, V>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }
	auto as_pair() const { return std::pair<T, T>(*this); }
	auto as_complex() const { return std::complex<T>(*this); }

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

	AreaT dist2() const { return AreaT(x) * AreaT(x) + AreaT(y) * AreaT(y); }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return std::gcd(x,y); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }

	// Convenient free-functions, mostly for generic interop
	friend auto norm(const Point& a) { return a.dist2(); }
	friend auto abs(const Point& a) { return a.dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	friend auto arg(const Point& a) { return a.angle(); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend AreaT dot(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.x) + AreaT(a.y) * AreaT(b.y); }
	friend AreaT cross(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.y) - AreaT(a.y) * AreaT(b.x); }
	friend AreaT cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.dist2(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool lex_less(const Point& a, const Point& b) {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}

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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N_orig, Q; std::cin >> N_orig >> Q;
	std::vector<int64_t> v_lo; v_lo.reserve(N_orig);
	std::vector<int64_t> v_hi; v_hi.reserve(N_orig);
	v_lo.push_back(0);
	v_hi.push_back(0);
	for (int i = 0; i < N_orig; i++) {
		int a; std::cin >> a;
		if (a == -1) {
			v_lo.push_back(v_hi.back());
			v_hi.push_back(v_hi.back());
		} else {
			assert(a >= 0);
			v_hi.back() += a;
		}
	}

	int L = int(v_lo.size());

	using pt_t = Point<int64_t>;

	std::vector<int64_t> max_diff(L);

	[&](this auto&& self, int l, int r) -> void {
		assert(r - l >= 1);
		if (r - l == 1) {
			// just self
			setmax(max_diff[0], v_hi[l] - v_lo[l]);
			return;
		}
		int m = l + (r - l) / 2;
		self(l, m);
		self(m, r);
		std::vector<pt_t> los; los.reserve(m-l);
		{
			for (int i = l; i < m; i++) {
				pt_t cnd = {i, v_lo[i]};
				while (los.size() >= 2 && cross3(los.end()[-2], los.end()[-1], cnd) < 0) los.pop_back();
				los.push_back(cnd);
			}
		}
		std::vector<pt_t> his; his.reserve(r-m);
		{
			for (int i = m; i < r; i++) {
				pt_t cnd = {i, v_hi[i]};
				while (his.size() >= 2 && cross3(his.end()[-2], his.end()[-1], cnd) > 0) his.pop_back();
				his.push_back(cnd);
			}
		}
		assert(!los.empty() && !his.empty());
		std::vector<pt_t> lo_gaps; lo_gaps.reserve(los.size() - 1);
		for (int i = 0; i+1 < int(los.size()); i++) {
			lo_gaps.push_back(los[i+1] - los[i]);
		}
		std::reverse(lo_gaps.begin(), lo_gaps.end());
		std::vector<pt_t> hi_gaps; hi_gaps.reserve(his.size() - 1);
		for (int i = 0; i+1 < int(his.size()); i++) {
			hi_gaps.push_back(his[i+1] - his[i]);
		}
		pt_t cur = his.front() - los.back();
		auto lo_it = lo_gaps.begin(), hi_it = hi_gaps.begin();
		setmax(max_diff[cur.x], cur.y);
		while (lo_it != lo_gaps.end() || hi_it != hi_gaps.end()) {
			auto cmp = angle_cmp(pt_t{1, 0});
			// we want the bigger one
			if (lo_it != lo_gaps.end() && (hi_it == hi_gaps.end() || cmp(*hi_it, *lo_it))) {
				cur += *lo_it;
				++lo_it;
			} else {
				cur += *hi_it;
				++hi_it;
			}
			setmax(max_diff[cur.x], cur.y);
		}
	}(0, L);
	std::vector<pt_t> hull; hull.reserve(L);
	for (int i = 0; i < L; i++) {
		pt_t cnd = {i, max_diff[i]};
		while (hull.size() >= 2 && cross3(hull.end()[-2], hull.end()[-1], cnd) > 0) hull.pop_back();
		hull.push_back(cnd);
	}

	for (int q = 0; q < Q; q++) {
		int64_t c; std::cin >> c;
		pt_t dir{-c, 1};
		int mi = 0;
		int ma = int(hull.size());
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			auto a = dot(hull[md-1], dir);
			auto b = dot(hull[md-0], dir);
			if (a > b) {
				ma = md;
			} else {
				mi = md;
			}
		}
		std::cout << dot(hull[mi], dir) << '\n';
	}

	return 0;
}
