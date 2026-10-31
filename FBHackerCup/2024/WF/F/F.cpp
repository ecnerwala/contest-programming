#include<bits/stdc++.h>

// number of integer solutions to Ax + By <= C and x,y >= 0
inline long long lattice_cnt(long long A, long long B, long long C) {
	using ll = long long;

	assert(A >= 0 && B >= 0);
	if (C < 0) return 0;

	assert(A > 0 && B > 0);
	if (A > B) std::swap(A, B);
	assert(A <= B);

	ll ans = 0;
	while (C >= 0) {
		assert(0 < A && A <= B);

		ll k = B/A;
		ll l = B%A;
		assert(B == k * A + l);

		ll f = C/B;
		ll e = C%B / A;
		ll g = C%B % A;
		assert(C == f * B + e * A + g);
		assert(C == (f * k + e) * A + f * l + g);

		// either x + ky <= f*k+e
		// i.e. 0 <= x <= (f-y) * k + e
		// or x >= fk + e + 1 - ky
		// and Ax + (Ak+l) y <= C = (fk + e + 1) A + fl - A + g
		// Let z = x - (fk + e + 1 - ky)
		// Az + A(fk + e + 1 - ky) + Aky + ly <= C = A (fk + e + 1) + fl - A + g
		// Az + ly <= fl - A + g

		ans += (f+1) * (e+1) + (f+1) * f / 2 * k;

		C = f*l - A + g;
		B = A;
		A = l;
	}
	return ans;
}

// count the number of 0 <= (a * x % m) < c for 0 <= x < n
inline long long mod_count(long long a, long long m, long long c, long long n) {
	assert(m > 0);
	if (n == 0) return 0;

	a %= m; if (a < 0) a += m;

	long long extraC = c / m; c %= m;
	if (c < 0) extraC--, c += m;
	assert(0 <= c && c < m);

	long long ans = extraC * n;

	long long extraN = n / m; n %= m;
	if (n < 0) extraN--, n += m;
	assert(0 <= n && n < m);

	if (extraN) {
		ans += extraN * (lattice_cnt(m, a+m, (a+m) * (m-1)) - lattice_cnt(m, a+m, (a+m) * (m-1) - c));
	}

	if (n) {
		// we want solutions to 0 <= a(N-1-x) - my < c with 0 <= x <= N-1
		// a * (N-1) >= ax + my > a * (N-1) - c
		ans += lattice_cnt(m, a+m, (a+m) * (n-1)) - lattice_cnt(m, a+m, (a+m) * (n-1) - c);
	}

	return ans;
}

inline long long mod_count_range(long long a, long long m, long long clo, long long chi, long long nlo, long long nhi) {
	return mod_count(a, m, chi, nhi) - mod_count(a, m, chi, nlo) - mod_count(a, m, clo, nhi) + mod_count(a, m, clo, nlo);
}

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		using pt_t = Point<int64_t>;

		int N; int64_t R; cin >> N >> R;
		std::vector<std::array<pt_t, 3>> triangles(N);
		for (auto& t : triangles) for (auto& p : t) cin >> p;

		cerr << "Case #" << case_num << " " << N << ' ' << R << '\n';

		auto in_circ = [&](pt_t p) -> bool {
			return p.dist2() <= R * R;
		};


		using num = modnum<int(1e9)+7>;
		num ans = 0;

		for (auto t : triangles) {
			if (cross3(t[0], t[1], t[2]) < 0) std::swap(t[1], t[2]);
			if (in_circ(t[0]) && in_circ(t[1]) && in_circ(t[2])) {
				auto line_pts = [&](pt_t a) -> int64_t {
					return std::gcd(a.x, a.y);
				};
				int64_t pts = cross3(t[0], t[1], t[2]) + line_pts(t[1] - t[0]) + line_pts(t[2] - t[1]) + line_pts(t[0] - t[2]) + 2;
				assert(!(pts & 1));
				pts /= 2;
				ans += pts;
			} else {
				// this triangle intersects the circle
				while (!in_circ(t[0])) {
					std::rotate(t.begin(), t.begin() + 1, t.end());
				}
				assert(in_circ(t[0]));
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
