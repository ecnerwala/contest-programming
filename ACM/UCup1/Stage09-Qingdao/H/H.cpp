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

template <typename T> int signum(T v) { return (v > 0) - (v < 0); }
int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {

		using pt_t = Point<int64_t>;
		int N_STONES; cin >> N_STONES;
		pt_t S, T; cin >> S >> T;
		std::array<pt_t, 2> M; cin >> M[0] >> M[1];
		std::swap(M[0], M[1]); // left side of the mirror
		std::array<pt_t, 3> B; cin >> B[0] >> B[1] >> B[2];

		using seg_t = std::array<pt_t, 2>;

		auto solve = [&]() -> std::optional<double> {
			if (cross3(B[0], B[1], B[2]) < 0) std::swap(B[1], B[2]);

			auto strictly_inside_B = [&](pt_t p) -> bool {
				if (cross3(p, B[1], B[2]) > 0 && cross3(B[0], p, B[2]) > 0 && cross3(B[0], B[1], p) > 0) return true;
				return true;
			};

			// requires either points equal or no 3 points collinear
			auto strictly_intersects = [&](seg_t a, seg_t b) -> bool {
				if (a[0] == b[0] || a[0] == b[1] || a[1] == b[0] || a[1] == b[1]) return false;
				auto a0 = signum(cross3(a[0], b[0], b[1]));
				auto a1 = signum(cross3(a[1], b[0], b[1]));
				auto b0 = signum(cross3(b[0], a[0], a[1]));
				auto b1 = signum(cross3(b[1], a[0], a[1]));
				return a0 * a1 == -1 && b0 * b1 == -1;
			};

			auto seg_hits_obstacle = [&](seg_t a) -> bool {
				return strictly_intersects(a, M) || strictly_intersects(a, {B[0], B[1]}) || strictly_intersects(a, {B[1], B[2]}) || strictly_intersects(a, {B[2], B[0]});
			};

			auto solve_path = [&]() -> double {
				std::vector<pt_t> pts({S, T, M[0], M[1], B[0], B[1], B[2]});
				std::vector<double> best(pts.size(), 1e99);
				best[0] = 0;
				for (int z = 0; z < int(pts.size()); z++) {
					for (int i = 0; i < int(pts.size()); i++) {
						for (int j = 0; j < int(pts.size()); j++) {
							if (i == j) continue;
							if (!seg_hits_obstacle({pts[i], pts[j]})) {
								best[j] = std::min(best[j], best[i] + abs(pts[i] - pts[j]));
							}
						}
					}
				}
				return best[1];
			};

			// If there's 1 stone, just do the shortest path
			if (N_STONES == 1) {
				return solve_path();
			}

			// Otherwise, we have to do at least one trip while seeing both;
			// this means that either they can see each other, or they can see each other in the mirror

			// If they can see each other, we're immediately done
			if (!seg_hits_obstacle({S, T})) {
				return (S - T).dist() * (N_STONES * 2 - 1);
			}

			// Otherwise, they must see each other in the mirror initially

			// They have to be on the correct side of the mirror

			// ok, time to do proper mirror stuff
			// Recenter the mirror to the positive x-axis, so that positive is actual and negative is fake
			S -= M[0];
			T -= M[0];
			B[0] -= M[0];
			B[1] -= M[0];
			B[2] -= M[0];
			M[1] -= M[0];
			M[0] -= M[0];

			S = S.unrotate(M[1]);
			T = T.unrotate(M[1]);
			B[0] = B[0].unrotate(M[1]);
			B[1] = B[1].unrotate(M[1]);
			B[2] = B[2].unrotate(M[1]);
			M[0] = M[0].unrotate(M[1]);
			M[1] = M[1].unrotate(M[1]);

			assert(M[0] == pt_t(0, 0));
			assert(M[1].x > 0 && M[1].y == 0);

			if (S.y < 0 || T.y < 0) return std::nullopt;

			pt_t S_ref = S; S_ref.y *= -1;
			pt_t T_ref = T; T_ref.y *= -1;

			if (seg_hits_obstacle({S, T_ref}) || seg_hits_obstacle({S_ref, T})) {
				return std::nullopt;
			}

			return 1e99;
		};

		auto ans = solve();
		if (ans) {
			cout << fixed << setprecision(12) << *ans << '\n';
		} else {
			cout << "-1" << '\n';
		}
	}

	return 0;
}
