#include <bits/stdc++.h>

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
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

struct mod_goldilocks {
	static constexpr uint64_t MOD = 0xffffffff00000001ull;
	struct is_reduced_tag {};
	uint64_t v;
	mod_goldilocks() : v(0) {}
	mod_goldilocks(uint64_t v_, is_reduced_tag) : v(v_) { assert(v < MOD); }
	mod_goldilocks(int64_t a) : v(a < 0 ? a+MOD : a) {}
	mod_goldilocks(uint64_t a) : v(a >= MOD ? a-MOD : a) {}
	mod_goldilocks(__int128_t a) : v(a % MOD < 0 ? uint64_t(MOD - a % MOD) : uint64_t(a % MOD)) {}
	mod_goldilocks(__uint128_t a) : v(uint64_t(a % MOD)) {}

	static mod_goldilocks from_reduced(uint64_t v) {
		return mod_goldilocks(v, is_reduced_tag{});
	}

	explicit operator uint64_t () const { return v; }
	int64_t as_signed() const { return MOD-v > v ? v : int64_t(v - MOD); }
	friend std::ostream& operator << (std::ostream& out, mod_goldilocks n) { return out << uint64_t(n); }

	friend bool operator == (mod_goldilocks a, mod_goldilocks b) { return a.v == b.v; }
	friend bool operator != (mod_goldilocks a, mod_goldilocks b) { return a.v != b.v; }

	mod_goldilocks operator+ () const { return *this; }

	mod_goldilocks neg() const { return from_reduced(v ? MOD-v : 0); }
	friend mod_goldilocks neg(const mod_goldilocks& m) { return m.neg(); }
	mod_goldilocks operator- () const { return neg(); }

	mod_goldilocks& operator ++ () {
		++ v;
		if (v == MOD) v = 0;
		return *this;
	}
	mod_goldilocks& operator -- () {
		if (v == 0) v = MOD;
		-- v;
		return *this;
	}
	mod_goldilocks& operator += (mod_goldilocks o) {
		uint64_t nv = v - (MOD-o.v);
		v = (nv > v) ? nv+MOD : nv;
		return *this;
	}
	mod_goldilocks& operator -= (mod_goldilocks o) {
		uint64_t nv = v - o.v;
		v = (nv > v) ? nv+MOD : nv;
		return *this;
	}
	mod_goldilocks& operator *= (mod_goldilocks o) {
		__uint128_t prod = __uint128_t(v) * __uint128_t(o.v);
		uint64_t hi = uint64_t(prod >> 64);
		uint64_t lo = uint64_t(prod);

		// hi <= (2^32 - 1)^2 = 2^64 - 2 * 2^32 + 1
		// hi_hi <= 2^32 - 2
		uint32_t hi_hi = uint32_t(hi >> 32);
		uint32_t hi_lo = uint32_t(hi);

		uint64_t v_hi_lo = ((uint64_t(hi_lo) << 32) - hi_lo);
		uint64_t neg_hi = (v_hi_lo ? MOD - v_hi_lo : 0) + hi_hi;

		// 0 <= r < 2^64
		uint64_t r = lo - neg_hi;
		if (r > lo) r += MOD;
		if (r >= MOD) r -= MOD;
		v = r;
		return *this;
	}
	// TODO: inv

	friend mod_goldilocks operator ++ (mod_goldilocks& a, int) { mod_goldilocks r = a; ++a; return r; }
	friend mod_goldilocks operator -- (mod_goldilocks& a, int) { mod_goldilocks r = a; --a; return r; }
	friend mod_goldilocks operator + (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) += b; }
	friend mod_goldilocks operator - (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) -= b; }
	friend mod_goldilocks operator * (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) *= b; }
};

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

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

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		cerr << "case " << N << ' ' << K << '\n';
		std::vector<Point<int>> pts(N);
		for (int i = 0; i < N; i++) {
			cin >> pts[i];
		}
		std::vector<int> side(N, -1);
		for (int st = 0; st < N; st++) {
			if (side[st] == -1) {
				[&](this auto&& self, int cur, int d) -> void {
					if (side[cur] == -1) {
						side[cur] = d;
						for (int nxt = 0; nxt < N; nxt++) {
							if (norm(pts[nxt] - pts[cur]) == K) {
								self(nxt, !d);
							}
						}
					}
					assert(side[cur] == d);
				}(st, 0);
			}
		}
		std::vector<Point<int>> p0; p0.reserve(N);
		std::vector<Point<int>> p1; p1.reserve(N);
		for (int i = 0; i < N; i++) {
			if (side[i] == 0) p0.push_back(pts[i]);
			else if (side[i] == 1) p1.push_back(pts[i]);
			else assert(false);
		}
		if (p0.size() > p1.size()) std::swap(p0, p1);
		assert(int(p0.size()) <= int(p1.size()));
		int s0 = int(p0.size());
		int s1 = int(p1.size());
		std::vector<uint32_t> m1(s1);
		for (int i = 0; i < s1; i++) {
			for (int j = 0; j < s0; j++) {
				if (norm(p1[i] - p0[j]) == K) m1[i] += 1 << j;
			}
		}
		using num = modnum<998244353>;
		num ans = 0;
		for (int small_0 = 0; small_0 < (1 << s0); small_0++) {
			int l = __builtin_popcount(small_0);
			std::vector<num> dp(1 << l, num(1));
			std::vector<num> ndp(1 << l);
			for (auto m : m1) {
				// if m is small, we add this
				num w = __builtin_popcount(m & ~small_0) + 1;
				for (int a = 0; a < (1 << l); a++) {
					ndp[a] = dp[a] * w;
				}
				// if m is big, we go through and potentially accept each guy
				for (int i = 0, b = 0; i < s0; i++) {
					if ((small_0 >> i) & 1) {
						if ((m >> i) & 1) {
							// potentially set bit b
							for (int a = 0; a < (1 << l); a++) {
								if ((a >> b) & 1) continue;
								dp[a + (1 << b)] += dp[a];
							}
						}
						b++;
					}
				}
				for (int a = 0; a < (1 << l); a++) {
					dp[a] += ndp[a];
				}
			}
			ans += dp.back();
		}
		ans--;
		cout << ans << '\n';
	}

	return 0;
}
