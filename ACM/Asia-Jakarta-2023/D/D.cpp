#include <bits/stdc++.h>

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
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return __gcd(x,y); }
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	using pt_t = Point<int64_t>;
	using big_pt_t = Point<__int128_t>;

	std::vector<pt_t> pts(N);
	for (auto& p : pts) cin >> p;

	big_pt_t centroid = big_pt_t(pts[0] + pts[1] + pts[2]);
	auto cmp = [&](auto a, auto b) {
		return angle_less(big_pt_t{1, 0}, big_pt_t(a) * 3 - centroid, big_pt_t(b) * 3 - centroid);
	};
	for (int i = 0; i+1 < N; i++) {
		if (!cmp(pts[i], pts[i+1])) {
			std::rotate(pts.begin(), pts.begin() + ((i+1)%N), pts.end());
			break;
		}
	}
	for (int i = 0; i+1 < N; i++) { assert(cmp(pts[i], pts[i+1])); }
	assert(!cmp(pts[N-1], pts[0]));

	auto get_tangents = [&](pt_t p) -> std::array<pt_t, 2> {
		// returns whether p is outside
		auto is_outside = [&](int i) -> bool {
			return cross3(pts[i%N], pts[(i+1)%N], p) <= 0;
		};
		auto it = int(std::lower_bound(pts.begin(), pts.end(), 0, [&](pt_t a, int) -> bool {
			return angle_less(big_pt_t{1, 0}, big_pt_t(a) * 3 - centroid, big_pt_t(p) * 3 - centroid);
		}) - pts.begin());
		if (it == 0) it = N;
		it--;
		assert(is_outside(it));
		auto jt = int(std::lower_bound(pts.begin(), pts.end(), 0, [&](pt_t a, int) -> bool {
			return angle_less(big_pt_t{1, 0}, big_pt_t(a) * 3 - centroid, centroid - big_pt_t(p) * 3);
		}) - pts.begin());
		if (jt == 0) jt = N;
		jt--;
		assert(!is_outside(jt));

		// we go jt -> it -> jt
		std::array<pt_t, 2> res;
		for (int z = 0; z < 2; z++) {
			int lo = (z == 0 ? jt : it);
			int hi = (z == 0 ? it : jt);
			if (hi < lo) hi += N;
			assert(is_outside(lo) == z);
			assert(is_outside(hi) == !z);
			while (hi - lo > 1) {
				int md = lo + (hi - lo) / 2;
				if (is_outside(md) == z) lo = md;
				else hi = md;
			}
			res[z] = pts[hi%N];
		}
		return res;
	};

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		pt_t a, b; cin >> a >> b;
		auto [a_lo, a_hi] = get_tangents(a);
		auto [b_lo, b_hi] = get_tangents(b);

		assert(cross3(a_lo, a, a_hi) >= 0);
		if (angle_between(a_lo - a, a_hi - a, b - a)) {
		}
	}

	return 0;
}
