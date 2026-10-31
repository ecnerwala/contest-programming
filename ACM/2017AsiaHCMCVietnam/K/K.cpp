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

using Pt = Point<int64_t>;

bool inside(const std::vector<Pt>& A, Pt p) {
	if (A.empty()) return false;
	for (int i = 0; i < int(A.size()); i++) {
		Pt x = A[i], y = A[(i+1) % int(A.size())];
		if (area(x, y, p) < 0) return false;
	}
	return true;
}

std::vector<Pt> get_real_hull(std::vector<Pt> A) {
	using namespace std;
	int N = int(A.size());
	vector<Pt> hull;
	sort(A.begin(), A.end(), [](const Pt& a, const Pt& b) { return tie(a.x, a.y) < tie(b.x, b.y); });
	{
		vector<Pt> upper; upper.reserve(N);
		vector<Pt> lower; lower.reserve(N);
		for (auto p : A) {
			while (lower.size() >= 2 && area(lower.end()[-2], lower.end()[-1], p) <= 0) lower.pop_back();
			lower.push_back(p);
			while (upper.size() >= 2 && area(upper.end()[-2], upper.end()[-1], p) >= 0) upper.pop_back();
			upper.push_back(p);
		}
		hull = std::move(lower);
		hull.insert(hull.end(), upper.rbegin()+1, upper.rend()-1);
	}

	if (hull.size() >= 4) {
		return hull;
	}
	if (hull.size() <= 2) {
		return {};
	}
	assert(int(hull.size()) == 3);
	assert(area(hull[0], hull[1], hull[2]) > 0);

	for (Pt p : A) {
		if (p == hull[0] || p == hull[1] || p == hull[2]) continue;
		int64_t a2 = area(hull[0], hull[1], p);
		int64_t a0 = area(hull[1], hull[2], p);
		int64_t a1 = area(hull[2], hull[0], p);
		if (a2 > 0 && a0 > 0 && a1 > 0) return hull;
	}

	vector<Pt> big_hull;
	{
		vector<Pt> upper; upper.reserve(N);
		vector<Pt> lower; lower.reserve(N);
		for (auto p : A) {
			while (lower.size() >= 2 && area(lower.end()[-2], lower.end()[-1], p) < 0) lower.pop_back();
			lower.push_back(p);
			while (upper.size() >= 2 && area(upper.end()[-2], upper.end()[-1], p) > 0) upper.pop_back();
			upper.push_back(p);
		}
		big_hull = std::move(lower);
		big_hull.insert(big_hull.end(), upper.rbegin()+1, upper.rend()-1);
	}
	int i0 = int(find(big_hull.begin(), big_hull.end(), hull[0]) - big_hull.begin());
	int i1 = int(find(big_hull.begin(), big_hull.end(), hull[1]) - big_hull.begin());
	int i2 = int(find(big_hull.begin(), big_hull.end(), hull[2]) - big_hull.begin());
	assert(i0 == 0);
	vector<Pt> s0(big_hull.begin() + i0 + 1, big_hull.begin() + i1);
	vector<Pt> s1(big_hull.begin() + i1 + 1, big_hull.begin() + i2);
	vector<Pt> s2(big_hull.begin() + i2 + 1, big_hull.end());
	if (!s0.empty() && !s1.empty() && !s2.empty()) return hull;
	if (s0.empty() + s1.empty() + s2.empty() >= 2) return {};
	if (s0.empty()) {
		return {hull[0], hull[1], s1.back(), s2.front()};
	} else if (s1.empty()) {
		return {hull[1], hull[2], s2.back(), s0.front()};
	} else if (s2.empty()) {
		return {hull[2], hull[0], s0.back(), s1.front()};
	} else assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<Pt> A(N); for (auto& a : A) cin >> a;

	A = get_real_hull(std::move(A));

	int ans = 0;

	int M; cin >> M;
	while (M--) {
		Pt p; cin >> p;
		ans += inside(A, p);
	}
	cout << ans << '\n';

	return 0;
}
