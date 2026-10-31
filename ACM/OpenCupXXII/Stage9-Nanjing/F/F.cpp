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

bool lex_cmp(const pt_t& a, const pt_t& b) {
	return std::pair<int64_t, int64_t>(a) < std::pair<int64_t, int64_t>(b);
}

std::vector<pt_t> hull(std::vector<pt_t> pts) {
	assert(!pts.empty());
	std::sort(pts.begin(), pts.end(), lex_cmp);
	std::vector<pt_t> lower; lower.reserve(pts.size());
	std::vector<pt_t> upper; upper.reserve(pts.size());
	for (pt_t cur : pts) {
		while (lower.size() >= 2 && cross3(lower.end()[-2], lower.end()[-1], cur) < 0) lower.pop_back();
		while (upper.size() >= 2 && cross3(upper.end()[-2], upper.end()[-1], cur) > 0) upper.pop_back();
		lower.push_back(cur);
		upper.push_back(cur);
	}
	assert(lower.front() == upper.front());
	assert(lower.back() == upper.back());
	if (upper.size() == 1) {
		// do nothing
	} else {
		lower.insert(lower.end(), upper.rbegin() + 1, upper.rend() - 1);
	}
	return lower;
}

bool is_good(const std::vector<pt_t>& hull) {
	if (hull.size() <= 2) return false;
	int N = int(hull.size());
	for (int i = 0; i < N; i++) {
		if (cross3(hull[i], hull[(i+1)%N], hull[(i+2)%N]) <= 0) {
			return false;
		}
	}
	return true;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<pt_t> P(N);
		for (int i = 0; i < N; i++) {
			cin >> P[i];
		}

		double ans = 0;

		{
			std::vector<pt_t> p_sorted = P;

			p_sorted.push_back(pt_t(0,0));
			std::sort(p_sorted.begin(), p_sorted.end(), lex_cmp);
			auto outer = hull(p_sorted);

			double perim = 0;
			if (is_good(outer)) {
				for (int i = 0; i < int(outer.size()); i++) {
					perim += (outer[(i+1)%int(outer.size())] - outer[i]).dist();
				}


				std::vector<pt_t> inner_pts; inner_pts.reserve(N+1);
				{
					std::sort(outer.begin(), outer.end(), lex_cmp);
					auto it = outer.begin();
					for (pt_t p : p_sorted) {
						if (it != outer.end() && p == *it) ++it;
						else inner_pts.push_back(p);
					}
				}
				inner_pts.push_back(pt_t(0, 0));
				auto inner = hull(inner_pts);
				if (is_good(inner) && inner.size() == inner_pts.size()) {
					for (int i = 0; i < int(inner.size()); i++) {
						perim += (inner[(i+1)%int(inner.size())] - inner[i]).dist();
					}
					ans = std::max(ans, perim);
				}
			}
		}

		{
			std::sort(P.begin(), P.end(), angle_cmp(pt_t(-1, 0)));
			for (int i = 0; i < N; i++) {
				if (same_dir(P[i], P[(i+1)%N])) {
					goto done;
				}
			}

			std::vector<bool> is_bad(N);
			int num_bad = 0;
			for (int i = 0; i < N; i++) {
				if (cross3(P[i], P[(i+1)%N], P[(i+2)%N]) <= 0) {
					is_bad[(i+1)%N] = true;
					num_bad ++;
				}
			}

			double base_perim = 0;
			for (int i = 0; i < N; i++) {
				base_perim += (P[i] - P[(i+1)%N]).dist();
			}

			for (int i = 0, j = 1; j < N; ) {
				if (j <= i+1) {
					j++;
					continue;
				}
				if ((j+1) % N == i) {
					i++;
					continue;
				}
				if (is_reflex(P[(j+1)%N], P[i])) {
					j++;
					continue;
				}
				if (is_reflex(P[(i+1)%N], P[j])) {
					i++;
					continue;
				}
				// we can try this point out
				{
					int cur_bad = num_bad - is_bad[i] - is_bad[(i+1)%N] - is_bad[j] - is_bad[(j+1)%N];
					if (cur_bad == 0) {
						double cnd = base_perim;
						cnd -= (P[i] - P[(i+1)%N]).dist();
						cnd -= (P[j] - P[(j+1)%N]).dist();
						cnd += P[i].dist() + P[(i+1)%N].dist();
						cnd += P[j].dist() + P[(j+1)%N].dist();
						ans = std::max(ans, cnd);
					}
				}
				if (is_reflex(P[(i+1)%N], P[(j+1)%N])) {
					i++;
				} else {
					j++;
				}
			}
		}

done:
		cout << fixed << setprecision(11) << ans << '\n';
	}

	return 0;
}
