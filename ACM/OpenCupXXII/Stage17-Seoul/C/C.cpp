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
	friend bool operator < (Point a, Point b) { return std::tie(a.x, a.y) < std::tie(b.x, b.y); }

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	using pt_t = Point<int64_t>;
	std::vector<pt_t> P(N);
	for (auto& p : P) {
		cin >> p;
		p.x *= 2, p.y *= 2;
	}

	std::vector<pt_t> by_x = P;
	std::sort(by_x.begin(), by_x.end(), [](pt_t a, pt_t b) -> bool {
		return a.x < b.x;
	});
	std::vector<pt_t> by_y = P;
	std::sort(by_y.begin(), by_y.end(), [](pt_t a, pt_t b) -> bool {
		return a.y < b.y;
	});

	std::map<std::array<pt_t, 2>, std::pair<std::array<pt_t, 2>, int>> mem;

	std::vector<int> x1; x1.reserve(N);
	std::vector<int> y1; y1.reserve(N);
	std::vector<int> x2; x2.reserve(N);
	std::vector<int> y2; y2.reserve(N);

	auto get_median = [&](std::vector<int>& v) -> int {
		std::sort(v.begin(), v.end());
		assert(!v.empty());
		int ans;
		if (v.size() & 1) {
			ans = v[v.size() / 2];
		} else {
			ans = (v[v.size() / 2 - 1] + v[v.size() / 2]) / 2;
		}
		v.clear();
		return ans;
	};

	auto solve = std::y_combinator([&](auto self, pt_t p1, pt_t p2) -> std::pair<std::array<pt_t, 2>, int> {
		assert(p1 != p2);
		if (p2 < p1) std::swap(p1, p2);
		assert(p1 < p2);

		{
			auto it = mem.find({p1, p2});
			if (it != mem.end()) {
				return it->second;
			}
		}

		for (auto p : by_x) {
			if (norm(p - p1) <= norm(p - p2)) {
				x1.push_back(p.x);
			} else {
				x2.push_back(p.x);
			}
		}
		for (auto p : by_y) {
			if (norm(p - p1) <= norm(p - p2)) {
				y1.push_back(p.y);
			} else {
				y2.push_back(p.y);
			}
		}
		pt_t np1 = {get_median(x1), get_median(y1)};
		pt_t np2 = {get_median(x2), get_median(y2)};
		if (np2 < np1) std::swap(np1, np2);
		if (np1 == p1 && np2 == p2) {
			return mem[{p1, p2}] = {{p1, p2}, 1};
		} else {
			auto res = self(np1, np2);
			res.second++;
			return mem[{p1, p2}] = res;
		}
	});

	std::map<std::array<pt_t, 2>, std::pair<int64_t, int>> ans;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			// check if these two are representative
			auto it = solve(P[i], P[j]);
			auto& v = ans[it.first];
			v.first += it.second;
			v.second ++;
		}
	}

	cout << fixed << setprecision(20);
	for (auto [pts, v] : ans) {
		cout << pts[0].x*0.5 << ' ' << pts[0].y*0.5 << ' ' << pts[1].x*0.5 << ' ' << pts[1].y*0.5 << ' ' << double(v.first) / double(v.second) << '\n';
	}

	return 0;
}
