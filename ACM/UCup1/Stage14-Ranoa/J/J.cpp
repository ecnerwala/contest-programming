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

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

template <typename tag> struct dynamic_modnum {
private:
#if __cpp_inline_variables >= 201606
	// C++17 and up
	inline static int MOD_ = 0;
	inline static uint64_t BARRETT_M = 0;
#else
	// NB: these must be initialized out of the class by hand:
	//   static int dynamic_modnum<tag>::MOD = 0;
	//   static int dynamic_modnum<tag>::BARRETT_M = 0;
	static int MOD_;
	static uint64_t BARRETT_M;
#endif

public:
	// Make only the const-reference public, to force the use of set_mod
	static constexpr int const& MOD = MOD_;

	// Barret reduction taken from KACTL:
	/**
	 * Author: Simon Lindholm
	 * Date: 2020-05-30
	 * License: CC0
	 * Source: https://en.wikipedia.org/wiki/Barrett_reduction
	 * Description: Compute $a \% b$ about 5 times faster than usual, where $b$ is constant but not known at compile time.
	 * Returns a value congruent to $a \pmod b$ in the range $[0, 2b)$.
	 * Status: proven correct, stress-tested
	 * Measured as having 4 times lower latency, and 8 times higher throughput, see stress-test.
	 * Details:
	 * More precisely, it can be proven that the result equals 0 only if $a = 0$,
	 * and otherwise lies in $[1, (1 + a/2^64) * b)$.
	 */
	static void set_mod(int mod) {
		assert(mod > 0);
		MOD_ = mod;
		BARRETT_M = (uint64_t(-1) / MOD);
	}
	static uint32_t barrett_reduce_partial(uint64_t a) {
		return uint32_t(a - uint64_t((__uint128_t(BARRETT_M) * a) >> 64) * MOD);
	}
	static int barrett_reduce(uint64_t a) {
		int32_t res = int32_t(barrett_reduce_partial(a) - MOD);
		return (res < 0) ? res + MOD : res;
	}

	struct mod_reader {
		friend std::istream& operator >> (std::istream& i, mod_reader) {
			int mod; i >> mod;
			dynamic_modnum::set_mod(mod);
			return i;
		}
	};
	static mod_reader MOD_READER() {
		return mod_reader();
	}

private:
	int v;

public:

	dynamic_modnum() : v(0) {}
	dynamic_modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const dynamic_modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, dynamic_modnum& n) { int64_t v_; in >> v_; n = dynamic_modnum(v_); return in; }

	friend bool operator == (const dynamic_modnum& a, const dynamic_modnum& b) { return a.v == b.v; }
	friend bool operator != (const dynamic_modnum& a, const dynamic_modnum& b) { return a.v != b.v; }

	dynamic_modnum inv() const {
		dynamic_modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend dynamic_modnum inv(const dynamic_modnum& m) { return m.inv(); }
	dynamic_modnum neg() const {
		dynamic_modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend dynamic_modnum neg(const dynamic_modnum& m) { return m.neg(); }

	dynamic_modnum operator- () const {
		return neg();
	}
	dynamic_modnum operator+ () const {
		return dynamic_modnum(*this);
	}

	dynamic_modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	dynamic_modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	dynamic_modnum& operator += (const dynamic_modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	dynamic_modnum& operator -= (const dynamic_modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	dynamic_modnum& operator *= (const dynamic_modnum& o) {
		v = barrett_reduce(int64_t(v) * int64_t(o.v));
		return *this;
	}
	dynamic_modnum& operator /= (const dynamic_modnum& o) {
		return *this *= o.inv();
	}

	friend dynamic_modnum operator ++ (dynamic_modnum& a, int) { dynamic_modnum r = a; ++a; return r; }
	friend dynamic_modnum operator -- (dynamic_modnum& a, int) { dynamic_modnum r = a; --a; return r; }
	friend dynamic_modnum operator + (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) += b; }
	friend dynamic_modnum operator - (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) -= b; }
	friend dynamic_modnum operator * (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) *= b; }
	friend dynamic_modnum operator / (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) /= b; }
};

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
}

struct point {
	int a;
	point() : a(0) {}
	explicit point(int a_) : a(a_) { assert(a >= -1); }

	explicit operator bool () { return bool(a); }

	// This is useful so you can directly do array indices
	/* implicit */ operator int() const { return a; }

	point c(bool z) const {
		return point((a<<1)|z);
	}

	point operator [] (bool z) const {
		return c(z);
	}

	point p() const {
		return point(a>>1);
	}

	friend std::ostream& operator << (std::ostream& o, const point& p) { return o << int(p); }

	template <typename F> void for_each(F f) const {
		for (int v = a; v > 0; v >>= 1) {
			f(point(v));
		}
	}

	template <typename F> void for_parents_down(F f) const {
		// strictly greater than 0
		for (int L = log_2(a); L > 0; L--) {
			f(point(a >> L));
		}
	}

	template <typename F> void for_parents_up(F f) const {
		for (int v = a >> 1; v > 0; v >>= 1) {
			f(point(v));
		}
	}

	point& operator ++ () { ++a; return *this; }
	point operator ++ (int) { return point(a++); }
	point& operator -- () { --a; return *this; }
	point operator -- (int) { return point(a--); }
};

struct range {
	int a, b;
	range() : a(1), b(1) {}
	range(int a_, int b_) : a(a_), b(b_) {
		assert(1 <= a && a <= b && b <= 2 * a);
	}
	explicit range(std::array<int, 2> r) : range(r[0], r[1]) {}

	explicit operator std::array<int, 2>() const {
		return {a,b};
	}

	const int& operator[] (bool z) const {
		return z ? b : a;
	}

	friend std::ostream& operator << (std::ostream& o, const range& r) { return o << "[" << r.a << ".." << r.b << ")"; }

	// Iterate over the range from outside-in.
	//   Calls f(point a)
	template <typename F> void for_each(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++));
			if (y & 1) f(point(--y));
		}
	}

	// Iterate over the range from outside-in.
	//   Calls f(point a, bool is_right)
	template <typename F> void for_each_with_side(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++), false);
			if (y & 1) f(point(--y), true);
		}
	}

	// Iterate over the range from left to right.
	//    Calls f(point)
	template <typename F> void for_each_l_to_r(F f) const {
		int anc_depth = log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = (-a) & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point(((a-1) >> i) + 1));
		}
		for (int v = b & anc_msk; v; ) {
			int i = log_2(v);
			f(point((b >> i) - 1));
			v ^= (1 << i);
		}
	}

	// Iterate over the range from right to left.
	//    Calls f(point)
	template <typename F> void for_each_r_to_l(F f) const {
		int anc_depth = log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = b & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point((b >> i) - 1));
		}
		for (int v = (-a) & anc_msk; v; ) {
			int i = log_2(v);
			f(point(((a-1) >> i) + 1));
			v ^= (1 << i);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = log_2((x-1) ^ y);
		for (int i = log_2(x); i > dx; i--) {
			f(point(x >> i));
		}
		for (int i = anc_depth; i > dy; i--) {
			f(point(y >> i));
		}
	}

	template <typename F> void for_parents_up(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = log_2((x-1) ^ y);
		for (int i = dx+1; i <= anc_depth; i++) {
			f(point(x >> i));
		}
		for (int v = y >> (dy+1); v; v >>= 1) {
			f(point(v));
		}
	}
};

struct in_order_layout {
	// Alias them in for convenience
	using point = seg_tree::point;
	using range = seg_tree::range;

	int N, S;
	in_order_layout() : N(0), S(0) {}
	in_order_layout(int N_) : N(N_), S(N ? next_pow_2(N) : 0) {}

	point get_point(int a) const {
		assert(0 <= a && a < N);
		a += S;
		return point(a >= 2 * N ? a - N : a);
	}

	range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		if (N == 0) return range();
		a += S, b += S;
		return range((a >= 2 * N ? 2*(a-N) : a), (b >= 2 * N ? 2*(b-N) : b));
	}

	range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_leaf_index(point pt) const {
		int a = int(pt);
		assert(N <= a && a < 2 * N);
		return (a < S ? a + N : a) - S;
	}

	std::array<int, 2> get_node_bounds(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? (x>>1) + N : x) - S, (y >= 2 * N ? (y>>1) + N : y) - S};
	}

	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		int l = __builtin_clz(2*a+1) - __builtin_clz(2*N-1);
		int x = (2*a+1) << l;
		assert(S <= x && x < 2*S);
		return (x >= 2 * N ? (x>>1) + N : x) - S;
	}

	int get_node_size(point pt) const {
		auto bounds = get_node_bounds(pt);
		return bounds[1] - bounds[0];
	}
};

struct circular_layout {
	// Alias them in for convenience
	using point = seg_tree::point;
	using range = seg_tree::range;

	int N;
	circular_layout() : N(0) {}
	circular_layout(int N_) : N(N_) {}

	point get_point(int a) const {
		assert(0 <= a && a < N);
		return point(N + a);
	}

	range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		if (N == 0) return range();
		return range(N + a, N + b);
	}

	range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_leaf_index(point pt) const {
		int a = int(pt);
		assert(N <= a && a < 2 * N);
		return a - N;
	}

	// Returns {x,y} so that 0 <= x < N and 1 <= y <= N
	// If the point is non-wrapping, then 0 <= x < y <= N
	std::array<int, 2> get_node_bounds(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int S = next_pow_2(N);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? x >> 1 : x) - N, (y > 2 * N ? y >> 1 : y) - N};
	}

	// Returns the split point of the node, such that 1 <= s <= N.
	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		return get_node_bounds(pt.c(0))[1];
	}

	int get_node_size(point pt) const {
		auto bounds = get_node_bounds(pt);
		int r = bounds[1] - bounds[0];
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	struct my_tag{};
	using num = dynamic_modnum<my_tag>;
	cin >> num::MOD_READER();
	num curve_a, curve_b; cin >> curve_a >> curve_b;
	
	struct pt_t {
		num L, A, I;
	};

	auto neg = [&](pt_t p) -> pt_t {
		return pt_t{p.L, p.I, p.A};
	};

	auto add = [&](pt_t p0, pt_t p1) -> pt_t {
		num L0 = p0.L, A0 = p0.A, I0 = p0.I;
		num L1 = p1.L, A1 = p1.A, I1 = p1.I;

		if (L1 == 0) return p0;
		if (L0 == 0) return p1;
		num B0 = (A0 + I0) * L1, B1 = (A1 + I1) * L0;
		num C0 = (A0 - I0) * L1, C1 = (A1 - I1) * L0;

		if (B0 == B1) {
			if (C0 + C1 == 0) {
				return {0, 3, -3};
			}
			num Sum = A0 + I0, Dif = A0 - I0;
			num B = 3 * Sum * Sum + curve_a * L0 * L0;
			num C = 2 * Dif * L0;
			num D = 2 * C * Sum * Dif;
			num E = B * B - 2 * D;
			num X = C * E, Y = B * (D - E) - 2 * C * C * Dif * Dif;
			return {2 * C * C * C, X + Y, X - Y};
		} else {
			num B = B0 - B1, C = C0 - C1, D = L0 * L1;
			num E = C * C * D - B * B * (B0 + B1);
			num X = B * E, Y = C * (B0 * B * B - E) - C0 * B * B * B;
			return {2 * B * B * B * D, X + Y, X - Y};
		}
	};

	auto sub = [&](pt_t p0, pt_t p1) -> pt_t {
		return add(p0, neg(p1));
	};

	int N; cin >> N;

	seg_tree::in_order_layout layout(N);
	std::vector<pt_t> seg(2*N);
	auto update_node = [&](seg_tree::point a) {
		seg[a] = add(seg[2*a], seg[2*a+1]);
	};
	for (int i = 0; i < N; i++) {
		seg_tree::point a = layout.get_point(i);
		cin >> seg[a].L >> seg[a].A >> seg[a].I;
	}
	for (seg_tree::point a(N-1); a > 0; a--) {
		update_node(a);
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int i; cin >> i;
			seg_tree::point a = layout.get_point(i);
			cin >> seg[a].L >> seg[a].A >> seg[a].I;
			a.for_parents_up(update_node);
		} else if (op == 2) {
			int l, r; cin >> l >> r;
			auto res = seg[layout.get_point(l)];
			layout.get_range(l+1, r).for_each([&](auto a) -> void {
				res = sub(res, seg[a]);
			});
			if (res.L == 0) {
				cout << -1 << '\n';
			} else {
				cout << res.A * res.I / (res.L * res.L) << '\n';
			}
		} else assert(false);
	}

	return 0;
}
