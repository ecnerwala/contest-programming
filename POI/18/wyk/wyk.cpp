#include <bits/stdc++.h>

// Copied from https://en.cppreference.com/w/cpp/numeric/bit_cast

template <class To, class From>
typename std::enable_if_t<
	sizeof(To) == sizeof(From) &&
	std::is_trivially_copyable_v<From> &&
	std::is_trivially_copyable_v<To>,
	To>
// constexpr support needs compiler magic
bit_cast(const From& src) noexcept
{
	static_assert(std::is_trivially_constructible_v<To>,
		"This implementation additionally requires destination type to be trivially constructible");

	To dst;
	std::memcpy(&dst, &src, sizeof(To));
	return dst;
}

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

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	friend T norm(const Point& a) { return a.x * a.x + a.y * a.y; }
	friend auto abs(const Point& p) { return std::hypot(p.x, p.y); }
	friend T unit(const Point& a) { if (a == Point()) return a; return a / abs(a); }

	friend T int_norm(const Point& a) { return __gcd(a.x,a.y); }
	friend T int_unit(const Point& a) { if (a == Point()) return a; return a / int_norm(a); }

	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T area(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Returns conj(a) * b
	friend Point rotation(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }

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

using Pt = Point<int>;

using int128_t = long double;

struct Circle {
	int np;
	std::array<Pt, 3> pts;

	Circle() {
		np = 0;
	}
	explicit Circle(Pt p1) {
		np = 1;
		pts[0] = p1;
	}
	Circle(Pt p1, Pt p2) {
		np = 2;
		pts[0] = p1;
		pts[1] = p2;
	}

	int64_t v;
	int64_t vx, vy;
	int128_t vc;
	Circle(Pt p1, Pt p2, Pt p3) {
		np = 3;
		pts = {p1, p2, p3};

		v = cross(Point<int64_t>(pts[1] - pts[0]), Point<int64_t>(pts[2] - pts[0]));

		int64_t n0 = norm(Point<int64_t>(pts[0]));
		int64_t n1 = norm(Point<int64_t>(pts[1]));
		int64_t n2 = norm(Point<int64_t>(pts[2]));
		vx = area(Point<int64_t>(n0, pts[0].y), Point<int64_t>(n1, pts[1].y), Point<int64_t>(n2, pts[2].y));
		vy = area(Point<int64_t>(pts[0].x, n0), Point<int64_t>(pts[1].x, n1), Point<int64_t>(pts[2].x, n2));

		vc = 0;
		vc += int128_t(n0) * int128_t(cross(Point<int64_t>(pts[1]), Point<int64_t>(pts[2])));
		vc += int128_t(n1) * int128_t(cross(Point<int64_t>(pts[2]), Point<int64_t>(pts[0])));
		vc += int128_t(n2) * int128_t(cross(Point<int64_t>(pts[0]), Point<int64_t>(pts[1])));

		assert(v != 0);
		if (v < 0) v = -v, vx = -vx, vy = -vy, vc = -vc;
	}

	bool contains(Pt p) {
		if (np == 0) {
			return false;
		} else if (np == 1) {
			return p == pts[0];
		} else if (np == 2) {
			return norm(Point<int64_t>(p - pts[0])) + norm(Point<int64_t>(p - pts[1])) <= norm(Point<int64_t>(pts[0] - pts[1]));
		} else if (np == 3) {
			return int128_t(v) * norm(Point<int64_t>(p)) - int128_t(vx) * p.x - int128_t(vy) * p.y - vc <= 0;
		} else assert(false);
	}

	// TODO: This could return a fraction if we really wanted
	double radius() {
		if (np == 0) {
			return -1;
		} else if (np == 1) {
			return 0;
		} else if (np == 2) {
			return abs(pts[0] - pts[1]) / 2;
		} else if (np == 3) {
			return abs(pts[1] - pts[0]) * abs(pts[2] - pts[0]) * abs(pts[2] - pts[1]) / double(2 * v);
		} else assert(false);
	}

	Point<double> center() {
		if (np == 0) {
			return {0,0};
		} else if (np == 1) {
			return Point<double>(pts[0]);
		} else if (np == 2) {
			return Point<double>(pts[0] + pts[1]) * 0.5;
		} else if (np == 3) {
			int64_t a = norm(Point<int64_t>(pts[2] - pts[1]));
			int64_t b = norm(Point<int64_t>(pts[2] - pts[0]));
			int64_t c = norm(Point<int64_t>(pts[1] - pts[0]));

			Point<int128_t> res;
			res += Point<int128_t>(Point<int64_t>(pts[0]) * a) * int128_t(b + c - a);
			res += Point<int128_t>(Point<int64_t>(pts[1]) * b) * int128_t(c + a - b);
			res += Point<int128_t>(Point<int64_t>(pts[2]) * c) * int128_t(a + b - c);

			Point<double> res_double(double(res.x), double(res.y));
			res_double /= double(2 * v);
			res_double /= double(2 * v);
			return res_double;
		} else assert(false);
	}
};

std::mt19937 mt(48);
auto get_mec(std::vector<Pt>& pts) -> Circle {
	std::shuffle(pts.begin(), pts.end(), mt);
	Circle cur;
	for (int i = 0; i < int(pts.size()); i++) {
		if (cur.contains(pts[i])) continue;
		cur = Circle(pts[i]);
		for (int j = 0; j < i; j++) {
			if (cur.contains(pts[j])) continue;
			cur = Circle(pts[i], pts[j]);
			for (int k = 0; k < j; k++) {
				if (cur.contains(pts[k])) continue;
				cur = Circle(pts[i], pts[j], pts[k]);
			}
		}
	}
	return cur;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<Pt> pts(N);
	for (auto& p : pts) cin >> p;

	std::vector<Pt> buf; buf.reserve(N);
	auto substr_mec = [&](int l, int r) -> Circle {
		buf.assign(pts.begin()+l, pts.begin()+r);
		return get_mec(buf);
	};

	std::vector<Point<double>> ans; ans.reserve(N);
	auto run_trial = [&](double hi, bool gen_ans = false) -> bool {
		int cnt = 0;
		for (int cur = 0; cur < N; ) {
			assert(cnt < M);
			if (cnt == M-1) {
				auto circ = substr_mec(cur, N);
				if (circ.radius() <= hi) {
					cnt++;
					if (gen_ans) {
						ans.push_back(circ.center());
					}
					return true;
				} else {
					return false;
				}
			}

			int mi = 1;
			int ma = 2;
			Circle last_mi(pts[cur]);

			while (cur + ma <= N) {
				auto circ = substr_mec(cur, cur + ma);
				if (circ.radius() <= hi) {
					last_mi = circ;
					mi = ma;
					ma *= 2;
				} else {
					break;
				}
			}
			ma = min(ma, N+1 - cur);
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				auto circ = substr_mec(cur, cur + md);
				if (circ.radius() <= hi) {
					last_mi = circ;
					mi = md;
				} else {
					ma = md;
				}
			}
			if (gen_ans) {
				ans.push_back(last_mi.center());
			}
			cur += mi;
			cnt++;
		}
		return true;
	};

	double lo = 0;
	double hi = 1e7;
	while (true) {
		auto ilo = bit_cast<uint64_t>(lo);
		auto ihi = bit_cast<uint64_t>(hi);
		if (ihi - ilo == 1) break;
		auto imd = ilo + (ihi-ilo) / 2;
		double md = bit_cast<double>(imd);
		if (run_trial(md)) {
			hi = md;
		} else {
			lo = md;
		}
	}

	run_trial(hi, true);
	cout << fixed << setprecision(15) << hi << '\n';
	cout << ans.size() << '\n';
	for (const auto& p : ans) {
		cout << fixed << setprecision(15) << p.x << ' ' << p.y << '\n';
	}

	return 0;
}
