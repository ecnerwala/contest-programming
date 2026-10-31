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

	constexpr bool TESTING = true;
	std::mt19937 mt(48);
	for (int case_num = 0; case_num < (TESTING ? 1000 : 1); case_num++) {
		using pt_t = Point<int64_t>;
		using big_pt_t = Point<__int128_t>;

		int N;
		std::vector<pt_t> pts;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(3, 100)(mt);
			//int X = int(pow(10, std::uniform_int_distribution<int>(1, 9)(mt)));
			int X = 5;
			pts.resize(N);
			for (auto& p : pts) {
				p.x = std::uniform_int_distribution<int>(-X, X)(mt);
				p.y = std::uniform_int_distribution<int>(-X, X)(mt);
			}
			std::sort(pts.begin(), pts.end(), [&](auto a, auto b) { return std::pair<int64_t, int64_t>(a) < std::pair<int64_t, int64_t>(b); });
			pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
			if (pts.size() < 3) continue;
			std::vector<pt_t> lower; lower.reserve(N);
			std::vector<pt_t> upper; upper.reserve(N);
			for (auto p : pts) {
				while (lower.size() >= 2 && cross3(lower.end()[-2], lower.end()[-1], p) <= 0) lower.pop_back();
				lower.push_back(p);
				while (upper.size() >= 2 && cross3(upper.end()[-2], upper.end()[-1], p) >= 0) upper.pop_back();
				upper.push_back(p);
			}
			lower.insert(lower.end(), upper.rbegin()+1, upper.rend()-1);
			pts = lower;
			N = int(pts.size());
			if (N < 3) continue;
			std::rotate(pts.begin(), pts.begin() + std::uniform_int_distribution<int>(0, N-1)(mt), pts.end());
		} else {
			cin >> N;
			pts.resize(N);
			for (auto& p : pts) cin >> p;
		}

		if (TESTING) {
			cerr << "Case #" << case_num << " " << "N = " << N << '\n';
			for (auto p : pts) { cerr << p.x << ' ' << p.y << '\n'; }
		}

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
			// whoops
			std::swap(res[0], res[1]);
			return res;
		};

		using float_pt_t = Point<double>;

		auto solve = [&](pt_t a, pt_t b) -> std::optional<double> {
			auto [a_lo, a_hi] = get_tangents(a);
			auto [b_lo, b_hi] = get_tangents(b);

			assert(cross(a_lo - a, a_hi - a) >= 0);
			assert(cross(b_lo - b, b_hi - b) >= 0);
			int a_between = angle_between(a_lo - a, a_hi - a, b - a);
			int b_between = angle_between(b_lo - b, b_hi - b, a - b);
			//cerr << "a_between = " << a_between << '\n';
			//cerr << "b_between = " << b_between << '\n';
			if (a_between == 1 && b_between == 1) {
				// then we need to try the two angles
				std::optional<double> ans;
				for (int z = 0; z < 2; z++) {
					// time to compute some ray-ray intersection
					pt_t x = z == 0 ? a : b;
					pt_t x_dir = z == 0 ? a_lo : b_lo;
					pt_t y = z == 0 ? b : a;
					pt_t y_dir = z == 0 ? b_hi : a_hi;
					assert(cross3(x, x_dir, y) > 0);
					assert(cross3(x, x_dir, y_dir) >= 0);
					assert(cross3(x, y_dir, y) > 0);
					assert(cross3(x_dir, y_dir, y) >= 0);
					if (cross3(x, x_dir, y) > cross3(x, x_dir, y_dir) && cross3(x, y_dir, y) > cross3(x_dir, y_dir, y)) {
						// they can intersect
						float_pt_t inter = float_pt_t(y) + float_pt_t(y_dir - y) * double(cross3(x, x_dir, y)) / double(cross3(x, x_dir, y) - cross3(x, x_dir, y_dir));
						double cnd = (float_pt_t(x) - inter).dist() + (float_pt_t(y) - inter).dist();
						if (!ans || cnd < *ans) ans = cnd;
					}
				}
				return ans;
			} else {
				return (a-b).dist();
			}
		};

		auto gen_pt = [&]() {
			while (true) {
				//int X = int(pow(10, std::uniform_int_distribution<int>(1, 9)(mt)));
				int X = 6;
				pt_t p(
					std::uniform_int_distribution<int>(-X, X)(mt),
					std::uniform_int_distribution<int>(-X, X)(mt)
				);
				bool inside = true;
				for (int i = 0; i < N; i++) {
					if (cross3(pts[i], pts[(i+1)%N], p) <= 0) {
						inside = false;
						break;
					}
				}
				if (!inside) return p;
			}
		};
		if (TESTING) {
			int Q = 100;
			for (int q = 0; q < Q; q++) {
				pt_t a = gen_pt();
				pt_t b = gen_pt();
				cerr << "query" << '\n';
				cerr << a.x << ' ' << a.y << '\n';
				cerr << b.x << ' ' << b.y << '\n';
				auto res = solve(a, b);
			}
		} else {
			int Q; cin >> Q;
			for (int q = 0; q < Q; q++) {
				pt_t a, b; cin >> a >> b;
				auto res = solve(a, b);
				if (res) {
					cout << fixed << setprecision(12) << *res << '\n';
				} else {
					cout << -1 << '\n';
				}
			}
		}
	}

	return 0;
}
