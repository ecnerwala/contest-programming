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

	int N, W; cin >> N >> W;

	struct seg_t {
		int x;
		int ylo, yhi;
	};
	std::vector<seg_t> segs(N);
	for (auto& [x, ylo, yhi] : segs) cin >> x >> ylo >> yhi;

	const double INFTY = 1e99;

	std::vector<std::pair<double, int>> all_evts;

	for (int i = 0; i < N; i++) {
		// when is this guy covered?
		std::vector<std::pair<double, int>> evts; evts.reserve(2*N);
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			auto si = segs[i];
			auto sj = segs[j];
			if (si.x == sj.x) {
				assert(si.yhi <= sj.ylo || sj.yhi <= si.ylo);
				continue;
			}
			using pt_t = Point<int64_t>;
			pt_t si_p(si.x, si.ylo);

			auto inter = [&](pt_t a, pt_t b) -> double {
				int64_t x1 = a.x, y1 = a.y;
				int64_t x2 = b.x, y2 = b.y;
				assert(x1 != x2);
				assert(y1 != y2);
				return double(y2 * x1 - x2 * y1) / double(y2 - y1);
			};
			// First, do s_lo
			double vlo = -INFTY, vhi = 0;
			{
				pt_t sj_lo(sj.x, sj.ylo);
				bool good_infty = si_p.y >= sj_lo.y;
				bool good_zero = cross(sj_lo, si_p) > 0;
				if (good_infty && good_zero) {
					// nothing
				} else if (good_infty) {
					vhi = std::min(vhi, inter(si_p, sj_lo));
				} else if (good_zero) {
					vlo = std::max(vlo, inter(si_p, sj_lo));
				} else {
					vlo = 0;
					vhi = -INFTY;
				}
			}
			{
				pt_t sj_hi(sj.x, sj.yhi);
				bool good_infty = si_p.y <= sj_hi.y;
				bool good_zero = cross(si_p, sj_hi) > 0;
				if (good_infty && good_zero) {
					// nothing
				} else if (good_infty) {
					vhi = std::min(vhi, inter(si_p, sj_hi));
				} else if (good_zero) {
					vlo = std::max(vlo, inter(si_p, sj_hi));
				} else {
					vlo = 0;
					vhi = -INFTY;
				}
			}
			if (vlo < vhi) {
				evts.push_back({vlo, -1});
				evts.push_back({vhi, +1});
			}
		}
		std::sort(evts.begin(), evts.end());
		int cur = 0;
		for (auto [x, v] : evts) {
			bool old_good = (cur < 0);
			cur += v;
			bool new_good = (cur < 0);
			if (new_good && !old_good) {
				all_evts.push_back({x, -1});
			} else if (old_good && !new_good) {
				all_evts.push_back({x, +1});
			}
		}
		assert(cur == 0);
	}
	if (N == 1) {
		cout << -1 << '\n';
		exit(0);
	}
	assert(N >= 2);
	std::sort(all_evts.begin(), all_evts.end());
	int cur = 0;
	double ans = 0;
	for (int i = 0; i < int(all_evts.size()); i++) {
		double x = all_evts[i].first;
		bool old_good = (-cur >= N-1);
		while (i < int(all_evts.size()) && all_evts[i].first == x) {
			cur += all_evts[i].second;
			i++;
		}
		bool new_good = (-cur >= N-1);
		if (new_good && !old_good) {
			if (x <= -INFTY/2) {
				cout << -1 << '\n';
				exit(0);
			}
			cerr << "start at " << x << '\n';
			ans -= x;
		}
		if (old_good && !new_good) {
			cerr << "end at " << x << '\n';
			ans += x;
		}
	}
	assert(cur == 0);

	cout << fixed << setprecision(10) << ans << '\n';

	return 0;
}
