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

	int N, Q; cin >> N >> Q;
	string S; cin >> S;

	using mnum = modnum<998244353>;
	using hsh_t = pairnum<mnum, mnum>;
	std::mt19937 mt(48);
	hsh_t BASE;
	BASE.u = mt();
	BASE.v = inv(BASE.u);
	std::vector<hsh_t> pow_base(N+1);
	pow_base[0] = 1;
	for (int i = 1; i <= N; i++) pow_base[i] = pow_base[i-1] * BASE;
	const hsh_t IBASE = BASE.inv();
	std::vector<hsh_t> ipow_base(N+1);
	ipow_base[0] = 1;
	for (int i = 1; i <= N; i++) ipow_base[i] = ipow_base[i-1] * IBASE;

	seg_tree::in_order_layout layout(N);
	struct seg_t {
		int own_diff = 0;

		int sz = 0;
		// hashes are left * BASE + right
		hsh_t whole = 0;

		// merged_diff == 0 means ignore, which means ch[0].sz == 0 or ch[1].sz == 0
		int merged_diff = 0;
		int mid_sz = 0;
		hsh_t mid_hash = 0;
	};

	std::vector<seg_t> seg(2*N);
	for (seg_tree::point a(1); a < N; a++) {
		int m = layout.get_node_split(a);
		seg[a].own_diff = S[m+1] - S[m];
		if (seg[a].own_diff >= 2) seg[a].own_diff -= 3;
		if (seg[a].own_diff <= -2) seg[a].own_diff += 3;
	}
	for (seg_tree::point a(N); a < 2*N; a++) {
		seg[a].sz = 1;
	}
	auto get_one_diff = std::y_combinator([&](auto self, seg_tree::point a, int idx) -> int {
		assert(0 <= idx && idx < seg[a].sz - 1);
		if (seg[a].merged_diff == 0) {
			return seg[a.c(0)].sz > seg[a].mid_sz ? self(a.c(0), idx) : self(a.c(1), idx + seg[a].mid_sz);
		}
		int l_sz = seg[a.c(0)].sz - seg[a].mid_sz - 1;
		if (idx == l_sz) {
			return seg[a].merged_diff;
		} else if (idx < l_sz) {
			return self(a.c(0), idx);
		} else {
			return self(a.c(1), idx - l_sz - 1 + seg[a].mid_sz);
		}
	});
	auto get_sum = std::y_combinator([&](auto self, seg_tree::point a, int lo, int hi) -> hsh_t {
		assert(0 <= lo && lo <= hi && hi <= seg[a].sz-1);
		if (lo == hi) return 0;
		if (lo == 0 && hi == seg[a].sz - 1) {
			return seg[a].whole;
		}
		if (seg[a].merged_diff == 0) {
			assert(seg[a.c(0)].sz == seg[a].mid_sz || seg[a.c(1)].sz == seg[a].mid_sz);
			if (seg[a.c(0)].sz == seg[a].mid_sz) {
				if (lo == 0) {
					return (self(a.c(1), lo, hi + seg[a].mid_sz) - seg[a].mid_hash * pow_base[hi - lo]);
				} else {
					return self(a.c(1), lo + seg[a].mid_sz, hi + seg[a].mid_sz);
				}
			} else if (seg[a.c(1)].sz == seg[a].mid_sz) {
				if (hi == seg[a].sz - 1) {
					return (self(a.c(0), lo, hi + seg[a].mid_sz) - seg[a].mid_hash) * ipow_base[seg[a].mid_sz];
				} else {
					return self(a.c(0), lo, hi);
				}
			}
			else assert(false);
		}
		int l = seg[a.c(0)].sz - seg[a].mid_sz - 1;
		int r = seg[a.c(1)].sz - seg[a].mid_sz - 1;
		assert(seg[a].sz - 1 == l + 1 + r);
		if (hi <= l) {
			if (hi == l) {
				return (self(a.c(0), lo, hi + seg[a].mid_sz) - seg[a].mid_hash) * ipow_base[seg[a].mid_sz];
			} else {
				return self(a.c(0), lo, hi);
			}
		}
		if (lo >= l+1) {
			if (lo == l+1) {
				return self(a.c(1), lo - (l+1), hi - (l+1) + seg[a].mid_sz) - seg[a].mid_hash * pow_base[hi - lo];
			} else {
				return self(a.c(1), lo - (l+1) + seg[a].mid_sz, hi - (l+1) + seg[a].mid_sz);
			}
		}
		hsh_t res = 0;
		res *= pow_base[l - lo];
		res += (self(a.c(0), lo, l + seg[a].mid_sz) - seg[a].mid_hash) * ipow_base[seg[a].mid_sz];
		res *= BASE;
		res += seg[a].merged_diff;
		res *= pow_base[hi - (l+1)];
		res += self(a.c(1), 0, hi - (l+1) + seg[a].mid_sz) - seg[a].mid_hash * pow_base[hi - (l+1)];
		return res;
	});
	auto update_node = [&](seg_tree::point a) -> void {
		assert(a < N);
		if (!seg[a.c(0)].sz) {
			seg[a].sz = seg[a.c(1)].sz;
			seg[a].whole = seg[a.c(1)].whole;
			seg[a].mid_sz = 0;
			seg[a].merged_diff = 0;
		}
		if (!seg[a.c(1)].sz) {
			seg[a].sz = seg[a.c(0)].sz;
			seg[a].whole = seg[a.c(0)].whole;
			seg[a].mid_sz = 0;
			seg[a].merged_diff = 0;
		}
		if (seg[a].own_diff == 0) {
			// we should binary search for the max overlap of the two children
			int mi = 0;
			int ma = std::min(seg[a.c(0)].sz-1, seg[a.c(1)].sz-1) + 1;
			hsh_t v_mi = 0;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				hsh_t v1 = get_sum(a.c(0), seg[a.c(0)].sz - 1 - md, seg[a.c(0)].sz);
				hsh_t v2 = get_sum(a.c(1), 0, md);
				if (v1.u + v2.v == 0 && v1.v + v2.u == 0) {
					mi = md;
					v_mi = v1;
				} else {
					ma = md;
				}
			}

			seg[a].mid_sz = mi + 1;
			seg[a].mid_hash = v_mi;

		} else {
			seg[a].mid_sz = 0;
			seg[a].mid_hash = 0;
		}

		seg[a].sz = seg[a.c(0)].sz + seg[a.c(1)].sz - 2 * seg[a].mid_sz;

		if (seg[a].mid_sz < seg[a.c(0)].sz && seg[a].mid_sz < seg[a.c(1)].sz) {
			seg[a].merged_diff =
				seg[a].mid_sz == 0 ? seg[a].own_diff :
				get_one_diff(a.c(0), seg[a.c(0)].sz - seg[a].mid_sz)
				+ get_one_diff(a.c(1), seg[a].mid_sz - 1);
			if (seg[a].merged_diff == 2) {
				seg[a].merged_diff -= 3;
			}
			if (seg[a].merged_diff == -2) {
				seg[a].merged_diff += 3;
			}
			assert(seg[a].merged_diff != 0);
			seg[a].whole = (seg[a.c(0)].whole - seg[a].mid_hash) * ipow_base[seg[a].mid_sz];
			seg[a].whole *= BASE;
			seg[a].whole += seg[a].merged_diff;
			seg[a].whole *= pow_base[seg[a.c(1)].sz - seg[a].mid_sz - 1];
			seg[a].whole += seg[a.c(1)].whole - seg[a].mid_hash * pow_base[seg[a.c(1)].sz - seg[a].mid_sz - 1];
		} else {
			seg[a].merged_diff = 0;
			if (seg[a.c(0)].sz > seg[a].mid_sz) {
				seg[a].whole = (seg[a.c(0)].whole - seg[a].mid_hash) * ipow_base[seg[a].mid_sz];
			} else if (seg[a.c(1)].sz > seg[a].mid_sz) {
				seg[a].whole = seg[a.c(1)].whole - seg[a].mid_hash * pow_base[seg[a.c(1)].sz - seg[a].mid_sz - 1];
			} else {
				seg[a].whole = 0;
			}
		}
	};

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int l, r; cin >> l >> r; l--;
			auto rng = layout.get_range(l, r);
			rng.for_parents_up([&](seg_tree::point a) -> void {
				int m = layout.get_node_split(a);
				if (m == l) {
					seg[a].own_diff += 1;
					if (seg[a].own_diff == 2) seg[a].own_diff -= 3;
				}
				if (m == r) {
					seg[a].own_diff -= 1;
					if (seg[a].own_diff == -2) seg[a].own_diff += 3;
				}
				update_node(a);
			});
		} else if (op == 2) {
			int l, r; cin >> l >> r; l--;
			auto rng = layout.get_range(l, r);
		} else assert(false);
	}

	return 0;
}
