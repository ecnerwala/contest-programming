#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

template <typename T, typename AreaT=T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U, typename V> explicit Point(const Point<U, V>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }
	auto as_pair() const { return std::pair<T, T>(*this); }
	auto as_complex() const { return std::complex<T>(*this); }

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

	AreaT dist2() const { return AreaT(x) * AreaT(x) + AreaT(y) * AreaT(y); }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return std::gcd(x,y); }
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

	friend AreaT dot(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.x) + AreaT(a.y) * AreaT(b.y); }
	friend AreaT cross(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.y) - AreaT(a.y) * AreaT(b.x); }
	friend AreaT cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.dist2(); }

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

	using pt_t = Point<int, int64_t>;

	int N; cin >> N;
	std::vector<pt_t> P(N);
	for (auto& p : P) cin >> p;
	// take the hull
	std::sort(P.begin(), P.end(), [&](auto a, auto b) { return lex_less(a, b); });

	assert(N >= 3);

	std::vector<int> hull; hull.reserve(N);
	{
		std::vector<int> upper; upper.reserve(N);
		std::vector<int> lower; lower.reserve(N);
		for (int i = 0; i < N; i++) {
			while (upper.size() >= 2 && cross3(P[upper.end()[-2]], P[upper.end()[-1]], P[i]) >= 0) upper.pop_back();
			while (lower.size() >= 2 && cross3(P[lower.end()[-2]], P[lower.end()[-1]], P[i]) <= 0) lower.pop_back();
			upper.push_back(i);
			lower.push_back(i);
		}
		std::reverse(upper.begin(), upper.end());
		hull.insert(hull.end(), lower.begin(), lower.end()-1);
		hull.insert(hull.end(), upper.begin(), upper.end()-1);
	}
	assert(hull.size() >= 3);
	if (hull.size() > 3) {
		//cerr << "bad " << '\n';
		cout << 0 << '\n';
		exit(0);
	}

	std::array<std::vector<int>, 3> sorts;
	for (int z = 0; z < 3; z++) {
		std::vector<int> cur(N);
		std::iota(cur.begin(), cur.end(), 0);
		cur.erase(cur.begin() + hull[z]);
		assert(int(cur.size()) == N-1);
		pt_t center = P[hull[z]];
		pt_t base = P[hull[(z+1)%3]] - center;
		std::sort(cur.begin(), cur.end(), [&](int a, int b) { return angle_less(base, P[a] - center, P[b] - center); });
		assert(cur.front() == hull[(z+1)%3]);
		assert(cur.back() == hull[(z+2)%3]);
		sorts[z] = std::move(cur);
	}

	std::array<std::vector<int>, 3> paths;
	for (int z = 0; z < 3; z++) {
		const auto& pref = sorts[(z+2)%3];
		const auto& suff = sorts[(z+1)%3];
		std::vector<int> cur; cur.reserve(N-1);
		for (int i = 0; i < int(pref.size()); i++) {
			if (pref[i] == suff[N-2-i]) cur.push_back(pref[i]);
			else break;
		}
		assert(cur.size() >= 1);
		assert(cur.front() == hull[z]);
		cerr << z << '\n';
		for (auto a : cur) { cerr << P[a] << ' '; } cerr << '\n';
		paths[z] = std::move(cur);
	}

	std::array<std::vector<std::array<int, 3>>, 3> constraints;
	for (int z = 0; z < 3; z++) {
		std::vector<std::array<int, 3>> cur(paths[z].size());
		cur[0] = {int(paths[0].size()), int(paths[1].size()), int(paths[2].size())};
		std::vector<bool> on_path(N);
		on_path[paths[z][0]] = true;
		for (int i = 1; i < int(paths[z].size()); i++) {
			on_path[paths[z][i]] = true;
			cur[i] = cur[i-1];
			std::vector<int> rest; rest.reserve(N);
			for (int o = 0; o < N; o++) {
				if (!on_path[o]) rest.push_back(o);
			}
			pt_t center = P[paths[z][i]];
			pt_t base = P[hull[(z+1)%3]] - center;
			std::sort(rest.begin(), rest.end(), [&](int a, int b) { return angle_less(base, P[a] - center, P[b] - center); });
			for (int d = 1; d <= 2; d++) {
				if (d == 2) std::reverse(rest.begin(), rest.end());
				int y = (z+d)%3;
				int k = 0;
				while (k < std::min(cur[i][y], int(rest.size())) && rest[k] == paths[y][k]) k++;
				cur[i][y] = k;
				if (d == 2) std::reverse(rest.begin(), rest.end());
			}
		}
		constraints[z] = std::move(cur);
	}
	for (int z = 0; z < 3; z++) {
		for (int y = 0; y < 3; y++) {
			if (y == z) continue;
			// propagate the paretos
			for (int i = 0; i < int(constraints[z].size()); i++) {
				if (constraints[z][i][y] < int(constraints[y].size())) {
					setmin(constraints[y][constraints[z][i][y]][z], i);
				}
			}
			for (int i = 1; i < int(constraints[y].size()); i++) {
				setmin(constraints[y][i][z], constraints[y][i-1][z]);
			}
			cerr << y << ' ' << z << '\n';
			for (int i = 0; i < int(constraints[y].size()); i++) { cerr << constraints[y][i][z] << ' '; } cerr << '\n';
		}
	}

	int64_t ans = 0;
	for (int v0 = 0; v0 < int(constraints[0].size()); v0++) {
		for (int v1 = 0; v1 < constraints[0][v0][1]; v1++) {
			ans += std::min(constraints[0][v0][2], constraints[1][v1][2]);
		}
	}
	cout << ans << '\n';

	return 0;
}
