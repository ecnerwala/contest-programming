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

using pt_t = Point<int64_t>;
using ld = long double;

struct frac_t {
	int64_t numer;
	int64_t denom;

	friend std::ostream& operator << (std::ostream& o, frac_t f) {
		return o << f.numer << '/' << f.denom;
	}

	friend auto operator <=> (frac_t a, frac_t b) {
		return __int128_t(a.numer) * b.denom <=> __int128_t(b.numer) * a.denom;
	}

	explicit operator ld () const {
		return ld(numer) / denom;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto get_ang = [&](const std::vector<pt_t>& a, pt_t ang) -> int {
		pt_t orig_base_ang = pt_t(1, 0);
		int mi = -1;
		int ma = int(a.size());
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			pt_t cnd_ang = a[(md+1 == int(a.size())) ? 0 : md+1] - a[md];
			if (angle_less(orig_base_ang, cnd_ang, ang)) {
				mi = md;
			} else {
				ma = md;
			}
		}
		return ma == int(a.size()) ? 0 : ma;
	};

	auto get_common_tangents_slow = [&](const std::vector<pt_t>& a, const std::vector<pt_t>& b) -> std::array<std::array<pt_t, 2>, 2> {
		pt_t base_ang = b[0] - a[0];
		int a_lo = get_ang(a, +base_ang);
		int a_hi = get_ang(a, -base_ang);
		assert(a_lo != a_hi);
		if (a_lo > a_hi) a_hi += int(a.size());

		int b_lo = get_ang(b, -base_ang);
		int b_hi = get_ang(b, +base_ang);
		assert(b_lo != b_hi);
		if (b_lo > b_hi) b_hi += int(b.size());

		auto pa = [&](int x) -> pt_t { return a[(x >= int(a.size())) ? x - int(a.size()) : x]; };
		auto pb = [&](int x) -> pt_t { return b[(x >= int(b.size())) ? x - int(b.size()) : x]; };

		std::array<pt_t, 2> ans_big{a[0], b[0]};
		std::array<pt_t, 2> ans_small{a[0], b[0]};

		for (int j = b_lo; j <= b_hi; j++) {
			for (int i = a_lo; i <= a_hi; i++) {
				std::array<pt_t, 2> cnd({pa(i), pb(j)});
				if (angle_less(-base_ang, ans_big[1] - ans_big[0], cnd[1] - cnd[0])) {
					ans_big = cnd;
				}
				if (angle_less(-base_ang, cnd[1] - cnd[0], ans_small[1] - ans_small[0])) {
					ans_small = cnd;
				}
			}
		}
		return {ans_big, ans_small};
	};

	auto get_common_tangents = [&](const std::vector<pt_t>& a, const std::vector<pt_t>& b) -> std::array<std::array<pt_t, 2>, 2> {
		pt_t base_ang = b[0] - a[0];
		int a_lo = get_ang(a, +base_ang);
		int a_hi = get_ang(a, -base_ang);
		assert(a_lo != a_hi);
		if (a_lo > a_hi) a_hi += int(a.size());

		int b_lo = get_ang(b, -base_ang);
		int b_hi = get_ang(b, +base_ang);
		assert(b_lo != b_hi);
		if (b_lo > b_hi) b_hi += int(b.size());

		auto pa = [&](int x) -> pt_t { return a[(x >= int(a.size())) ? x - int(a.size()) : x]; };
		auto pb = [&](int x) -> pt_t { return b[(x >= int(b.size())) ? x - int(b.size()) : x]; };

		std::array<pt_t, 2> ans_big{a[0], b[0]};
		std::array<pt_t, 2> ans_small{a[0], b[0]};

		for (int j = b_lo; j <= b_hi; j++) {
			pt_t cur = pb(j);
			{
				int mi = a_lo - 1, ma = a_hi;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					pt_t x = pa(md), y = pa(md+1);
					if (cross(base_ang, cur - y) < 0) {
						ma = md;
					} else if (cross(y - x, cur - y) < 0) {
						ma = md;
					} else {
						mi = md;
					}
				}
				std::array<pt_t, 2> cnd({pa(ma), cur});
				if (angle_less(-base_ang, ans_big[1] - ans_big[0], cnd[1] - cnd[0])) {
					ans_big = cnd;
				}
			}

			{
				int mi = a_lo - 1, ma = a_hi;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					pt_t x = pa(md), y = pa(md+1);
					if (cross(base_ang, cur - x) >= 0) {
						mi = md;
					} else if (cross(y - x, cur - x) >= 0) {
						mi = md;
					} else {
						ma = md;
					}
				}
				std::array<pt_t, 2> cnd({pa(ma), cur});
				if (angle_less(-base_ang, cnd[1] - cnd[0], ans_small[1] - ans_small[0])) {
					ans_small = cnd;
				}
			}
		}
		return {ans_big, ans_small};
	};

	int T; cin >> T;
	while (T--) {
		int N_MOON, L, R; cin >> N_MOON >> L >> R;
		//cerr << '\n' << "Start case " << N_MOON << ' ' << L << ' ' << R << '\n';

		auto read_polygon = [&]() -> std::vector<pt_t> {
			int K; cin >> K;
			std::vector<pt_t> P(K);
			for (auto& p : P) cin >> p;
			std::rotate(P.begin(), std::min_element(P.begin(), P.end(), [&](pt_t a, pt_t b) -> bool {
				return std::tie(a.y, a.x) < std::tie(b.y, b.x);
			}), P.end());
			return P;
		};
		auto get_inter = [&](pt_t p1, pt_t p2) -> std::array<frac_t, 2> {
			if (p1 == p2) {
				return {frac_t{L-1, 1}, frac_t{R+1, 1}};
			}
			// strict ccw halfplane of p1 -> p2, i.e. cross3(x, p1, p2) > 0
			bool l_inside = cross3(pt_t(L, 0), p1, p2) > 0;
			bool r_inside = cross3(pt_t(R, 0), p1, p2) > 0;
			if (l_inside && r_inside) {
				return {frac_t{L-1, 1}, frac_t{R+1, 1}};
			} else if (!l_inside && !r_inside) {
				return {frac_t{L-1, 1}, frac_t{L-1, 1}};
			} else {
				// only one of them
				assert(p1.y != p2.y);
				frac_t mid{p2.y * p1.x - p1.y * p2.x, p2.y - p1.y};
				if (mid.denom < 0) {
					mid.numer = -mid.numer;
					mid.denom = -mid.denom;
				}
				if (l_inside && !r_inside) {
					return {frac_t{L-1, 1}, mid};
				} else if (r_inside && !l_inside) {
					return {mid, frac_t{R+1, 1}};
				} else assert(false);
			}
		};

		auto intersect = [&](std::array<frac_t, 2> x, std::array<frac_t, 2> y, std::array<frac_t, 2> z) -> std::array<frac_t, 2> {
			return {std::max({x[0], y[0], z[0]}), std::min({x[1], y[1], z[1]})};
		};

		std::vector<pt_t> S = read_polygon();
		std::vector<std::pair<frac_t, int>> evts; evts.reserve(N_MOON * 2 + 1);
		for (int j = 0; j < N_MOON; j++) {
			//cerr << '\n' << "MOON " << j << '\n';
			std::vector<pt_t> M = read_polygon();

			// find the common tangents
			auto [t1, t2] = get_common_tangents(S, M);
			auto [s1, m1] = t1;
			auto [s2, m2] = t2;
			//cerr << "tangents " << s1 << ' ' << m1 << '\n';
			//cerr << "tangents " << s2 << ' ' << m2 << '\n';

			auto bad = intersect(get_inter(m1, s1), get_inter(m1, m2), get_inter(s2, m2));
			//cerr << "inter " << bad[0] << ' ' << bad[1] << '\n';
			if (bad[0] < bad[1]) {
				evts.push_back({bad[0], +1});
				evts.push_back({bad[1], -1});
			}
		}
		std::sort(evts.begin(), evts.end());
		evts.push_back({{R+1, 1}, +1});
		int bad_cnt = 0;
		frac_t prv_x{L-1, 1};
		bool has_good = false;
		ld ans = 0;
		for (auto [x, v] : evts) {
			//cerr << "interval " << prv_x << ' ' << x << ' ' << bad_cnt << ' ' << v << '\n';
			if (bad_cnt == 0) {
				// closed interval prv_x .. x is good
				if (x > frac_t{L, 1} && prv_x < frac_t{R, 1}) {
					has_good = true;
					frac_t lo = std::max(prv_x, frac_t{L, 1});
					frac_t hi = std::min(x, frac_t{R, 1});
					ans += ld(hi) - ld(lo);
				}
			}
			bad_cnt += v;
			prv_x = x;
		}
		if (has_good) {
			cout << fixed << setprecision(20) << ans << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
