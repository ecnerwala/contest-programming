#include <bits/stdc++.h>

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

/**
 * Author: Andrew He
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Papers about accuracy: http://www.daemonology.net/papers/fft.pdf, http://www.cs.berkeley.edu/~fateman/papers/fftvsothers.pdf
 * For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$.
 */

namespace ecnerwala {
namespace fft {

using std::swap;
using std::vector;
using std::min;
using std::max;

template<class T> int sz(T&& arg) { using std::size; return int(size(std::forward<T>(arg))); }
inline int nextPow2(int s) { return 1 << (s > 1 ? 32 - __builtin_clz(s-1) : 0); }

// Complex
template <typename dbl> struct cplx { /// start-hash
	dbl x, y;
	cplx(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) { }
	friend cplx operator+(cplx a, cplx b) { return cplx(a.x + b.x, a.y + b.y); }
	friend cplx operator-(cplx a, cplx b) { return cplx(a.x - b.x, a.y - b.y); }
	friend cplx operator*(cplx a, cplx b) { return cplx(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
	friend cplx conj(cplx a) { return cplx(a.x, -a.y); }
	friend cplx inv(cplx a) { dbl n = (a.x*a.x+a.y*a.y); return cplx(a.x/n,-a.y/n); }
};

// getRoot implementations
template <typename num> struct getRoot {
	static num f(int k) = delete;
};
template <typename dbl> struct getRoot<cplx<dbl>> {
	static cplx<dbl> f(int k) {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
		dbl a=2*M_PI/k;
		return cplx<dbl>(cos(a),sin(a));
	}
};
template <int MOD> struct primitive_root {
	static const int value;
};
template <> struct primitive_root<998244353> {
	static const int value = 3;
};
template <int MOD> struct getRoot<modnum<MOD>> {
	static modnum<MOD> f(int k) {
		assert((MOD-1)%k == 0);
		return pow(modnum<MOD>(primitive_root<MOD>::value), (MOD-1)/k);
	}
};

template <typename num> class fft {
	static vector<int> rev;
	static vector<num> rt;

public:
	static void init(int n);
	template <typename Iterator> static void go(Iterator begin, int n);

	static vector<num> scratch_a;
	static vector<num> scratch_b;
};

template <typename num> vector<int> fft<num>::rev;
template <typename num> vector<num> fft<num>::rt;
template <typename num> vector<num> fft<num>::scratch_a;
template <typename num> vector<num> fft<num>::scratch_b;

template <typename num> void fft<num>::init(int n) {
	if (n <= sz(rt)) return;
	rev.resize(n);
	for (int i = 0; i < n; i++) {
		rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
	}
	rt.reserve(n);
	while (sz(rt) < 2 && sz(rt) < n) rt.push_back(num(1));
	for (int k = sz(rt); k < n; k *= 2) {
		rt.resize(2*k);
		num z = getRoot<num>::f(2*k);
		for (int i = k/2; i < k; i++) {
			rt[2*i] = rt[i], rt[2*i+1] = rt[i]*z;
		}
	}
}

template <typename num> template <typename Iterator> void fft<num>::go(Iterator begin, int n) {
	init(n);
	int s = __builtin_ctz(sz(rev)/n);
	for (int i = 0; i < n; i++) {
		if (i < (rev[i]>>s)) {
			swap(*(begin+i), *(begin+(rev[i]>>s)));
		}
	}
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i < n; i += 2 * k) {
			Iterator it1 = begin + i, it2 = it1 + k;
			for (int j = 0; j < k; j++, ++it1, ++it2) {
				num t = rt[j+k] * *it2;
				*it2 = *it1 - t;
				*it1 = *it1 + t;
			}
		}
	}
}

template <typename num> struct fft_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<num>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);
		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		copy(ib, ib+szb, fb.begin());
		fill(fb.begin()+szb, fb.begin()+n, num(0));
		fft<num>::go(fa.begin(), n);
		fft<num>::go(fb.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);
		copy(fa.begin(), fa.begin()+s, io);
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <typename num>
struct fft_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0) return;
		int s = nextPow2(sza) * 2;
		fft<num>::init(s);
		if (sz(fa) < s) fa.resize(s);
		if (sz(fb) < s) fb.resize(s);
		fb[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			fill(fb.begin() + n, fb.begin() + 4 * n, num(0));
			n *= 2;
			copy(ia, ia+min(n,sza), fa.begin());
			fill(fa.begin()+min(n,sza), fa.begin()+2*n, 0);
			fft<num>::go(fb.begin(), 2*n);
			fft<num>::go(fa.begin(), 2*n);
			num d = inv(num(2*n));
			for (int i = 0; i < 2*n; i++) fb[i] = fb[i] * (2 - fa[i] * fb[i]) * d;
			reverse(fb.begin()+1, fb.begin()+2*n);
			fft<num>::go(fb.begin(), 2*n);
		}
		copy(fb.begin(), fb.begin()+sza, io);
	}
};

template <typename dbl>
struct fft_double_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<cplx<dbl>>& fa = fft<cplx<dbl>>::scratch_a;
		vector<cplx<dbl>>& fb = fft<cplx<dbl>>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cplx<dbl>>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		fill(fa.begin(), fa.begin() + n, 0);
		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i].x = *it; }
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fa[i].y = *it; }
		fft<cplx<dbl>>::go(fa.begin(), n);
		for (auto& x : fa) x = x * x;
		for (int i = 0; i < n; ++i) fb[i] = fa[(n-i)&(n-1)] - conj(fa[i]);
		fft<cplx<dbl>>::go(fb.begin(), n);
		{ auto it = io; for (int i = 0; i < s; ++i, ++it) *it = fb[i].y / (4*n); }
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <typename mnum>
struct fft_mod_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		using cnum = cplx<double>;
		vector<cnum>& fa = fft<cnum>::scratch_a;
		vector<cnum>& fb = fft<cnum>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cnum>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fa.begin()+sza, fa.begin() + n, 0);
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fb[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fb.begin()+szb, fb.begin() + n, 0);

		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		double r0 = 0.5 / n; // 1/2n
		for (int i = 0; i <= n/2; i++) {
			int j = (n-i)&(n-1);
			cnum g0 = (fb[i] + conj(fb[j])) * r0;
			cnum g1 = (fb[i] - conj(fb[j])) * r0;
			swap(g1.x, g1.y); g1.y *= -1;
			if (j != i) {
				swap(fa[j], fa[i]);
				fb[j] = fa[j] * g1;
				fa[j] = fa[j] * g0;
			}
			fb[i] = fa[i] * conj(g1);
			fa[i] = fa[i] * conj(g0);
		}
		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		using ll = long long;
		const ll m = mnum::MOD;
		auto it = io;
		for (int i = 0; i < s; ++i, ++it) {
			*it = mnum((ll(fa[i].x+0.5)
						+ (ll(fa[i].y+0.5) % m << 15)
						+ (ll(fb[i].x+0.5) % m << 15)
						+ (ll(fb[i].y+0.5) % m << 30)) % m);
		}
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <class multiplier, typename num>
struct multiply_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		if (sza == 0) return;
		int s = nextPow2(sza);
		vector<num> b(s,num(0));
		vector<num> tmp(2*s);
		b[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			// TODO: could be square instead of multiply
			multiplier::multiply(b.begin(),n,b.begin(),n,tmp.begin());
			int nn = min(sza,2*n);
			multiplier::multiply(tmp.begin(),nn,ia,nn,tmp.begin());
			for (int i = n; i < nn; i++) b[i] = -tmp[i];
			n = nn;
		}
		copy(b.begin(), b.begin()+sza, io);
	}
};

template <class multiplier, typename T> vector<T> multiply(const vector<T>& a, const vector<T>& b) {
	if (sz(a) == 0 || sz(b) == 0) return {};
	vector<T> r(max(0, sz(a) + sz(b) - 1));
	multiplier::multiply(begin(a), sz(a), begin(b), sz(b), begin(r));
	return r;
}

template <typename T> vector<T> fft_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_double_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_double_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_mod_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_mod_multiplier<T>, T>(a, b);
}

template <class multiplier, typename T> vector<T> square(const vector<T>& a) {
	if (sz(a) == 0) return {};
	vector<T> r(2 * sz(a) - 1);
	multiplier::square(begin(a), sz(a), begin(r));
	return r;
}
template <typename T> vector<T> fft_square(const vector<T>& a, const vector<T>& b) {
	return square<fft_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_double_square(const vector<T>& a, const vector<T>& b) {
	return square<fft_double_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_mod_square(const vector<T>& a, const vector<T>& b) {
	return square<fft_mod_multiplier<T>, T>(a, b);
}

template <class inverser, typename T> vector<T> inverse(const vector<T>& a) {
	vector<T> r(sz(a));
	inverser::inverse(begin(a), sz(a), begin(r));
	return r;
}
template <typename T> vector<T> fft_inverse(const vector<T>& a) {
	return inverse<fft_inverser<T>, T>(a);
}
template <typename T> vector<T> fft_double_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_double_multiplier<T>, T>, T>(a);
}
template <typename T> vector<T> fft_mod_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_mod_multiplier<T>, T>, T>(a);
}
/* namespace fft */ }

// Power series; these are assumed to be the min of the length
template <typename T, typename multiplier, typename inverser>
struct power_series : public std::vector<T> {
	using std::vector<T>::vector;

	int ssize() const {
		return int(this->size());
	}
	int len() const {
		return ssize();
	}
	int degree() const {
		return len() - 1;
	}
	void extend(int sz) {
		assert(sz >= ssize());
		this->resize(sz);
	}
	void shrink(int sz) {
		assert(sz <= ssize());
		this->resize(sz);
	}
	power_series& operator += (const power_series& o) {
		assert(len() == o.len());
		for (int i = 0; i < int(o.size()); i++) {
			(*this)[i] += o[i];
		}
		return *this;
	}
	friend power_series operator + (const power_series& a, const power_series& b) {
		power_series r(std::min(a.size(), b.size()));
		for (int i = 0; i < r.len(); i++) {
			r[i] = a[i] + b[i];
		}
		return r;
	}
	power_series& operator -= (const power_series& o) {
		assert(len() == o.len());
		for (int i = 0; i < int(o.size()); i++) {
			(*this)[i] -= o[i];
		}
		return *this;
	}
	friend power_series operator - (const power_series& a, const power_series& b) {
		power_series r(std::min(a.size(), b.size()));
		for (int i = 0; i < r.len(); i++) {
			r[i] = a[i] - b[i];
		}
		return r;
	}

	power_series& operator *= (const T& n) {
		for (auto& v : *this) v *= n;
	}
	friend power_series operator * (const power_series& a, const T& n) {
		power_series r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = a[i] * n;
		}
		return r;
	}
	friend power_series operator * (const T& n, const power_series& a) {
		power_series r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = n * a[i];
		}
		return r;
	}

	friend power_series operator * (const power_series& a, const power_series& b) {
		if (sz(a) == 0 || sz(b) == 0) return {};
		power_series r(std::max(0, sz(a) + sz(b) - 1));
		multiplier::multiply(begin(a), sz(a), begin(b), sz(b), begin(r));
		r.resize(std::min(a.size(), b.size()));
		return r;
	}
	power_series& operator *= (const power_series& o) {
		return *this = (*this) * o;
	}

	friend power_series inverse(power_series a) {
		power_series r(sz(a));
		inverser::inverse(begin(a), sz(a), begin(r));
		return r;
	}
	friend power_series deriv_shift(power_series a) {
		for (int i = 0; i < a.len(); i++) {
			a[i] *= i;
		}
		return a;
	}
	friend power_series integ_shift(power_series a) {
		assert(a[0] == 0);
		T f = 1;
		for (int i = 1; i < int(a.size()); i++) {
			a[i] *= f;
			f *= i;
		}
		f = inv(f);
		for (int i = int(a.size()) - 1; i > 0; i--) {
			a[i] *= f;
			f *= i;
		}
		return a;
	}
	friend power_series deriv_shift_log(power_series a) {
		auto r = deriv_shift(a);
		return r * inverse(a);
	}
	friend power_series poly_log(power_series a) {
		assert(a[0] == 1);
		return integ_shift(deriv_shift_log(std::move(a)));
	}
	friend power_series poly_exp(power_series a) {
		assert(a.size() >= 1);
		assert(a[0] == 0);
		power_series r(1, T(1));
		while (r.size() < a.size()) {
			int n_sz = std::min(int(r.size()) * 2, int(a.size()));
			r.resize(n_sz);
			power_series v(a.begin(), a.begin() + n_sz);
			v -= poly_log(r);
			v[0] += 1;
			r *= v;
		}
		return r;
	}
	friend power_series poly_pow_monic(power_series a, int64_t k) {
		if (a.empty()) return a;
		assert(a.size() >= 1);
		assert(a[0] == 1);
		a = poly_log(a);
		a *= k;
		return poly_exp(a);
	}
	friend power_series poly_pow(power_series a, int64_t k) {
		int st = 0;
		while (st < a.len() && a[st] == 0) st++;
		if (st == a.len()) return a;

		power_series r(a.begin() + st, a.end());
		T leading_coeff = r[0];
		r *= inv(leading_coeff);
		r = poly_pow_monic(r, k);
		r *= pow(leading_coeff, k);
		r.insert(r.begin(), size_t(st), T(0));
		return r;
	}

	friend power_series to_newton_sums(const power_series& a, int deg) {
		auto r = log_deriv_shift(a);
		r[0] = deg;
		for (int i = 1; i < int(r.size()); i++) r[i] = -r[i];
		return r;
	}
	friend power_series from_newton_sums(power_series S, int deg) {
		assert(S[0] == int(deg));
		S[0] = 0;
		for (int i = 1; i < int(S.size()); i++) S[i] = -S[i];
		return poly_exp(integ_shift(std::move(S)));
	}
};

template <typename num> using power_series_fft = power_series<num, fft::fft_multiplier<num>, fft::fft_inverser<num>>;
template <typename num, typename multiplier> using power_series_with_multiplier = power_series<num, multiplier, fft::multiply_inverser<multiplier, num>>;
template <typename num> using power_series_fft_mod = power_series_with_multiplier<num, fft::fft_mod_multiplier<num>>;
template <typename num> using power_series_fft_double = power_series_with_multiplier<num, fft::fft_double_multiplier<num>>;

// TODO: Use iterator traits to deduce value type?
template <typename base_iterator, typename value_type> struct add_into_iterator {
	base_iterator base;
	add_into_iterator() : base() {}
	add_into_iterator(base_iterator b) : base(b) {}
	add_into_iterator& operator * () { return *this; }
	add_into_iterator& operator ++ () { base.operator ++ (); return *this; }
	add_into_iterator& operator ++ (int) { auto temp = *this; operator ++ (); return temp; }
	auto operator = (value_type v) { base.operator * () += v; }
};

// TODO: Use iterator traits to deduce value type?
template <typename base_iterator, typename value_type> struct add_double_into_iterator {
	base_iterator base;
	add_double_into_iterator() : base() {}
	add_double_into_iterator(base_iterator b) : base(b) {}
	add_double_into_iterator& operator * () { return *this; }
	add_double_into_iterator& operator ++ () { base.operator ++ (); return *this; }
	add_double_into_iterator& operator ++ (int) { auto temp = *this; operator ++ (); return temp; }
	auto operator = (value_type v) { base.operator * () += 2 * v; }
};

template <typename num, typename multiplier> struct online_multiplier {
	int N; int i;
	std::vector<num> f, g;
	std::vector<num> res;

	// Computes the first 2N terms of the product
	online_multiplier(int N_) : N(N_), i(0), f(N), g(N), res(2*N+1, num(0)) {}

	num peek() {
		return res[i];
	}

	void push(num v_f, num v_g) {
		assert(i < N);
		f[i] = v_f;
		g[i] = v_g;
		if (i == 0) {
			res[i] += v_f * v_g;
		} else {
			res[i] += v_f * g[0];
			res[i] += f[0] * v_g;
			// TODO: We could do this second half more lazily, since it only affects res[i+1]...
			for (int p = 1; (i & (p-1)) == (p-1); p <<= 1) {
				int lo1 = p;
				int lo2 = i + 1 - p;
				multiplier::multiply(
					// TODO: We can cache FFT([f,g].begin() + p, p)
					f.begin() + lo1, p,
					g.begin() + lo2, p,
					add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
				);
				if (i == 2*p-1) break;
				// TODO: Don't recompute if squaring
				multiplier::multiply(
					f.begin() + lo2, p,
					g.begin() + lo1, p,
					add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
				);
			}
		}
		i++;
	}

	num back() {
		return res[i-1];
	}
};

template <typename num, typename multiplier> struct online_squarer {
	int N; int i;
	std::vector<num> f;
	std::vector<num> res;

	// Computes the first 2N terms of the product
	online_squarer(int N_) : N(N_), i(0), f(N), res(2*N+1, num(0)) {}

	num peek() {
		return res[i];
	}

	void push(num v_f) {
		assert(i < N);
		f[i] = v_f;
		if (i == 0) {
			res[i] += v_f * v_f;
		} else {
			res[i] += 2 * v_f * f[0];
			// TODO: We could do this second half more lazily, since it only affects res[i+1]...
			for (int p = 1; (i & (p-1)) == (p-1); p <<= 1) {
				int lo1 = p;
				int lo2 = i + 1 - p;
				if (i == 2*p-1) {
					multiplier::square(
						// TODO: We can cache FFT([f,g].begin() + p, p)
						f.begin() + lo1, p,
						add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
					);
					break;
				} else {
					multiplier::multiply(
						// TODO: Use cached FFT
						f.begin() + lo1, p,
						f.begin() + lo2, p,
						add_double_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
					);
				}
			}
		}
		i++;
	}

	num back() {
		return res[i-1];
	}
};

/* namespace ecnerwala */ }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, B, C; cin >> A >> B >> C;

	int N; cin >> N;

	int A_orig = A;
	B -= A, C -= A;
	A = 0;

	int X = C + N;
	seg_tree::in_order_layout layout(X);

	return 0;
}
