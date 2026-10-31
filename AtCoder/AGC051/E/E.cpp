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

	friend T int_norm(const Point& a) { return std::gcd(a.x,a.y); }
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


using pt_t = Point<int64_t>;

bool operator < (const pt_t& a, const pt_t& b) {
	return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

int64_t solve_slow(std::vector<pt_t> pts) {
	std::set<pt_t> S;
	for (auto p : pts) {
		S.insert(p*2);
	}

	while (true) {
		std::set<pt_t> NS = S;
		for (auto a : S) {
			for (auto b : S) {
				pt_t c = (a + b);
				if (c.x & 1) continue;
				if (c.y & 1) continue;
				NS.insert(c / 2);
			}
		}
		if (NS == S) break;
		S = NS;
	}
	int64_t ans = 0;
	for (auto it : S) {
		if (it.x % 2 == 0 && it.y % 2 == 0) {
			ans++;
			std::cerr << it << '\n';
		}
	}
	return ans;
}

int64_t solve(std::vector<pt_t> pts) {
	using namespace std;
	int N = int(pts.size());

	for (int i = 1; i < N; i++) pts[i] -= pts[0];
	pts[0] -= pts[0];

	vector<pt_t> basis;

	auto div_2 = [&](pt_t a) -> pt_t {
		if (a == pt_t{0,0}) return a;
		if (a.x == 0) return {0, a.y >> __builtin_ctzll(a.y)};
		if (a.y == 0) return {a.x >> __builtin_ctzll(a.x), 0};
		int l = min(__builtin_ctzll(a.x), __builtin_ctzll(a.y));
		return {a.x >> l, a.y >> l};
	};

	auto reduce2 = [&](pt_t a, pt_t b) -> pt_t {
		assert(norm(a) != 0);
		int64_t x = dot(a, b) * 2 / norm(a);
		x += x % 2;
		x /= 2;
		b -= x * a;
		return div_2(b);
	};

	auto reduce = [&]() {
		while (basis.size() >= 3) {
			sort(basis.begin(), basis.end(), [&](const pt_t& a, const pt_t& b) { return norm(a) < norm(b); });
			if (norm(basis[0]) == 0) {
				basis.erase(basis.begin());
				break;
			}
			basis[2] = reduce2(basis[1], basis[2]);
			basis[1] = reduce2(basis[0], basis[1]);
			basis[2] = reduce2(basis[0], basis[2]);
		}
		assert(basis.size() == 2);
		while (true) {
			if (norm(basis[0]) > norm(basis[1])) swap(basis[0], basis[1]);
			{
				auto d0 = div_2(basis[0]);
				if (d0 != basis[0]) {
					basis[0] = d0;
					continue;
				}
			}
			{
				auto d1 = div_2(basis[1] - basis[0]);
				if (norm(d1) < norm(basis[1])) {
					basis[1] = d1;
					continue;
				}
			}
			{
				auto b1 = reduce2(basis[0], basis[1]);
				assert(norm(b1) <= norm(basis[1]));
				if (norm(b1) < norm(basis[1])) {
					basis[1] = b1;
					continue;
				}
			}
			break;
		}
	};

	basis.push_back(div_2(pts[1]));
	basis.push_back(div_2(pts[2]));
	reduce();
	for (int i = 3; i < N; i++) {
		basis.push_back(div_2(pts[i]));
		reduce();
	}

	{
		int64_t a = (basis[0].x & 1) << 1 | (basis[0].y & 1);
		int64_t b = (basis[1].x & 1) << 1 | (basis[1].y & 1);
		assert(a != 0 && b != 0 && a != b);
	}

	for (int i = 0; i < N; i++) {
		// write pts[i] as a linear combination of basis[0] and basis[1]
		int64_t d0 = cross(pts[i], basis[1]) / cross(basis[0], basis[1]);
		int64_t d1 = cross(pts[i], basis[0]) / cross(basis[1], basis[0]);
		assert(pts[i] == d0 * basis[0] + d1 * basis[1]);
		pts[i] = {d0, d1};
	}

	vector<pt_t> uhull; uhull.reserve(N);
	vector<pt_t> lhull; lhull.reserve(N);
	sort(pts.begin(), pts.end(), [&](const auto& a, const auto& b) { return tie(a.x, a.y) < tie(b.x, b.y); });
	for (int i = 0; i < N; i++) {
		while (uhull.size() >= 2 && area(uhull.end()[-2], uhull.end()[-1], pts[i]) >= 0) uhull.pop_back();
		uhull.push_back(pts[i]);
		while (lhull.size() >= 2 && area(lhull.end()[-2], lhull.end()[-1], pts[i]) <= 0) lhull.pop_back();
		lhull.push_back(pts[i]);
	}
	int64_t res = 0;
	for (int i = 0; i+1 < int(uhull.size()); i++) {
		res -= cross(uhull[i], uhull[i+1]);
		res -= int_norm(uhull[i+1] - uhull[i]);
		res += 2 * int64_t(1) << __builtin_ctzll(int_norm(uhull[i+1] - uhull[i]));
	}
	for (int i = 0; i+1 < int(lhull.size()); i++) {
		res += cross(lhull[i], lhull[i+1]);
		res -= int_norm(lhull[i+1] - lhull[i]);
		res += 2 * int64_t(1) << __builtin_ctzll(int_norm(lhull[i+1] - lhull[i]));
	}
	res += 2;

	return res/2;
}

void test() {
	using namespace std;
	std::mt19937 mt(48);
	for (int iter = 0; true; iter++) {
		int N = 4;
		vector<pt_t> pts(N);
		for (auto& p : pts) {
			p.x = mt() % 6;
			p.y = mt() % 6;
		}
		bool good = true;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				for (int k = j+1; k < N; k++) {
					if (area(pts[i], pts[j], pts[k]) == 0) good = false;
				}
			}
		}
		if (!good) continue;
		cerr << "Running " << iter << '\n';
		auto slo = solve_slow(pts);
		auto res = solve(pts);
		if (res != slo) {
			cerr << N << '\n';
			for (auto p : pts) {
				cerr << p.x << ' ' << p.y << '\n';
			}
			cerr << "Expected " << slo << '\n';
			cerr << "Got " << res << '\n';
			assert(false);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//if (true) test();

	int N; cin >> N;
	vector<pt_t> pts(N); for (auto& p : pts) cin >> p;

	auto ans = solve(pts);
	cout << ans << '\n';

	return 0;
}

// Roughly all intermediate points are integer; you're allowed some half
// integers, but there can only be 2 at each level, and they have to pair
// immediately

// We'll probably get a set of convex regions, each of which has a different lattice

// What infinite lattices are possible? We can probably just run Gaussian
