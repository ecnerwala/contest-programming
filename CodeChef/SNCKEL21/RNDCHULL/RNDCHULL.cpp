#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(ll(v) * ll(o.v) % MOD);
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {

		using num = modnum<998244353>;

		int N; cin >> N;
		std::vector<int> X(N);
		for (auto& x : X) cin >> x;
		std::vector<int> Y(N);
		for (auto& y : Y) cin >> y;

		//for (auto& y : Y) { y = -y; } std::reverse(Y.begin(), Y.end());
		//for (auto& x : X) { x = -x; } std::reverse(X.begin(), X.end());

		num ans = 0;

		for (int x1 = 0; x1 < N; x1++) {
			for (int x2 = x1+1; x2 < N; x2++) {
				for (int y1 = 0; y1 < N; y1++) {
					for (int y2 = y1+1; y2 < N; y2++) {
						{
							// increasing slope below
							num v = cross(Point(X[x2], Y[y2]), Point(X[x1], Y[y1]));
							int cnt = 0;
							for (int x = 0, y = 0; x < N; x++) {
								if (x == x1 || x == x2) continue;
								for (; y < N; y++) {
									if (y == y1 || y == y2) continue;
									int a = cross3(Point(X[x1], Y[y1]), Point(X[x], Y[y]), Point(X[x2], Y[y2]));
									if (a < 0) {
										break;
									} else if (a == 0 && x1 < x && x < x2) {
										break;
									}
									cnt++;
								}
								v *= cnt;
								if (cnt == 0) break;
								cnt--;
							}
							ans += v;
						}

						{
							// increasing slope above
							num v = cross(Point(X[x1], Y[y1]), Point(X[x2], Y[y2]));
							int cnt = 0;
							for (int x = N-1, y = N-1; x >= 0; x--) {
								if (x == x1 || x == x2) continue;
								for (; y >= 0; y--) {
									if (y == y1 || y == y2) continue;
									int a = cross3(Point(X[x1], Y[y1]), Point(X[x], Y[y]), Point(X[x2], Y[y2]));
									if (a > 0) {
										break;
									} else if (a == 0 && x1 < x && x < x2) {
										break;
									}
									cnt++;
								}
								v *= cnt;
								if (cnt == 0) break;
								cnt--;
							}
							ans += v;
						}

						{
							// decreasing slope below
							num v = cross(Point(X[x2], Y[y1]), Point(X[x1], Y[y2]));
							int cnt = 0;
							for (int x = N-1, y = 0; x >= 0; x--) {
								if (x == x1 || x == x2) continue;
								for (; y < N; y++) {
									if (y == y1 || y == y2) continue;
									int a = cross3(Point(X[x1], Y[y2]), Point(X[x], Y[y]), Point(X[x2], Y[y1]));
									if (a < 0) {
										break;
									} else if (a == 0 && x1 < x && x < x2) {
										break;
									}
									cnt++;
								}
								v *= cnt;
								if (cnt == 0) break;
								cnt--;
							}
							ans += v;
						}

						{
							// decreasing slope above
							num v = cross(Point(X[x1], Y[y2]), Point(X[x2], Y[y1]));
							int cnt = 0;
							for (int x = 0, y = N-1; x < N; x++) {
								if (x == x1 || x == x2) continue;
								for (; y >= 0; y--) {
									if (y == y1 || y == y2) continue;
									int a = cross3(Point(X[x1], Y[y2]), Point(X[x], Y[y]), Point(X[x2], Y[y1]));
									if (a > 0) {
										break;
									} else if (a == 0 && x1 < x && x < x2) {
										break;
									}
									cnt++;
								}
								v *= cnt;
								if (cnt == 0) break;
								cnt--;
							}
							ans += v;
						}
					}
				}
			}
		}

		cout << ans << '\n';
	}

	return 0;
}
