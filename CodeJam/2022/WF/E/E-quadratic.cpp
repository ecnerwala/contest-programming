#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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

template <typename T> int signum(T a) { return (a > 0) - (a < 0); }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		using pt_t = Point<int64_t>;
		int N; cin >> N;
		std::vector<pt_t> P(N);
		for (auto& p : P) cin >> p;

		std::vector<std::array<int, 3>> res;

		std::vector<bool> used(N, false);
		auto get_third = [&](int p0, int p1) {
			int p2_pos = -1;
			int p2_neg = -1;
			for (int i = 0; i < N; i++) {
				if (used[i]) continue;
				// area of p0, p1, i
				auto c3 = cross(P[i] - P[p1], P[p0] - P[p1]);
				if (c3 > 0) {
					if (p2_pos == -1) {
						p2_pos = i;
					} else {
						auto v = cross(P[p2_pos] - P[p1], P[i] - P[p1]);
						if (v > 0 || (v == 0 && norm(P[i] - P[p1]) < norm(P[p2_pos] - P[p1]))) {
							p2_pos = i;
						}
					}
				} else if (c3 < 0) {
					if (p2_neg == -1) {
						p2_neg = i;
					} else {
						auto v = cross(P[p2_neg] - P[p1], P[i] - P[p1]);
						if (v < 0 || (v == 0 && norm(P[i] - P[p1]) < norm(P[p2_neg] - P[p1]))) {
							p2_neg = i;
						}
					}
				} else {
					continue;
				}
			}
			if (p2_pos != -1) return p2_pos;
			else if (p2_neg != -1) return p2_neg;
			else return -1;
		};

		int num_pts = N;
		int num_line = 0;
		pt_t line_0, line_1;
		// Triangle count is bounded by (num_pts) / 3 and num_pts - num_line
		while (num_pts >= 3 && num_pts - num_line >= 1) {
			int p0 = -1;
			int p1 = -1;
			assert(num_line == 0 || (num_line >= num_pts - (num_pts / 3) - 1));
			if (num_pts == 6 && num_line == 3) {
				// There could be a 2nd big line that we need to take something from;
				// However, this is guaranteed possible since the other line is at most 4 anyways.
				std::vector<int> line_pts; line_pts.reserve(3);
				std::vector<int> other_pts; other_pts.reserve(3);
				for (int i = 0; i < N; i++) {
					if (used[i]) continue;
					if (cross3(line_0, line_1, P[i]) == 0) line_pts.push_back(i);
					else other_pts.push_back(i);
				}
				std::sort(line_pts.begin(), line_pts.end(), [&](int i, int j) { return lex_less(P[i], P[j]); });

				// First, we'll check for 2 non-touching triangles
				// inclusive intersection
				auto segment_inter = [&](pt_t a, pt_t b, pt_t c, pt_t d) {
					int vc = signum(cross3(a, b, c));
					int vd = signum(cross3(a, b, d));
					if (vc == 0 && vd == 0) {
						// line case
						int64_t la = dot(a, b - a);
						int64_t lb = dot(b, b - a);
						int64_t lc = dot(c, b - a);
						int64_t ld = dot(d, b - a);
						return std::max(std::min(la, lb), std::min(lc, ld)) <= std::min(std::max(la, lb), std::max(lc, ld));
					}
					return vc * vd != 1 && signum(cross3(c, d, a)) * signum(cross3(c, d, b)) != 1;
				};
				auto tri_good = [&](std::array<int, 3> x, std::array<int, 3> y) {
					if (cross3(P[x[0]], P[x[1]], P[x[2]]) == 0) return false;
					if (cross3(P[y[0]], P[y[1]], P[y[2]]) == 0) return false;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							if (segment_inter(P[x[i]], P[x[(i+1)%3]], P[y[j]], P[y[(j+1)%3]])) return false;
						}
					}
					return true;
				};
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 3; j++) {
						std::array<int, 3> c1 = {line_pts[i], line_pts[i+1], other_pts[j]};
						std::array<int, 3> c2 = {line_pts[(i+2)%3], other_pts[(j+1)%3], other_pts[(j+2)%3]};
						if (tri_good(c1, c2)) {
							res.push_back(c1);
							res.push_back(c2);
							goto found;
						}
					}
				}

				// Now, we have the 1 bad case: it must be a triangle with a point on each side.
				for (int j = 0; j < 3; j++) {
					int cnt = 0;
					for (int a : {0, 2}) {
						for (int b : {0, 1, 2}) {
							if (b == j) continue;
							if (cross3(P[line_pts[a]], P[other_pts[b]], P[other_pts[j]]) == 0) cnt ++;
						}
					}
					if (cnt == 2) {
						res.push_back({line_pts[0], line_pts[2], other_pts[j]});
						res.push_back({line_pts[1], other_pts[(j+1)%3], other_pts[(j+2)%3]});
						goto found;
					}
				}

				assert(false);
found:
				num_pts -= 6;
				num_line -= 3;
				break;
			}
			for (int i = 0; i < N; i++) {
				if (used[i]) continue;
				// If there are at least 2 line points, use them
				if (num_line >= 2 && cross3(line_0, line_1, P[i]) != 0) continue;
				if (p0 == -1 || lex_less(P[i], P[p0])) {
					p1 = p0;
					p0 = i;
				} else if (p1 == -1 || lex_less(P[i], P[p1])) {
					p1 = i;
				}
			}
			assert(p0 != -1 && p1 != -1);
			int p2 = get_third(p0, p1);
			if (p2 != -1) {
				used[p0] = used[p1] = used[p2] = true;
				res.push_back({p0, p1, p2});
				num_pts -= 3;
				// p0 and p1 are on the line, if necessary
				if (num_line >= 2) num_line -= 2;
			} else {
				assert(num_line == 0);
				num_line = num_pts;
				line_0 = P[p0], line_1 = P[p1];
				while (!res.empty() && (num_pts - num_line) < num_pts / 3) {
					for (int i : res.back()) {
						assert(used[i]);
						used[i] = false;
						num_pts ++;
						if (cross3(line_0, line_1, P[i]) == 0) num_line ++;
					}
					res.pop_back();
				}
			}
		}

		cout << "Case #" << case_num << ": " << int(res.size()) << '\n';
		for (auto [a, b, c] : res) {
			cout << a+1 << ' ' << b+1 << ' ' << c+1 << '\n';
		}
	}

	return 0;
}
