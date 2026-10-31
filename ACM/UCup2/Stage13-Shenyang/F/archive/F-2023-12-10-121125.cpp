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

using mnum1 = modnum<int(1e9)+7>;
using mnum2 = modnum<int(1e9)+9>;
using pnum = pairnum<mnum1, mnum2>;
struct hsh_t {
	pnum v = 0, l = 1;
	friend hsh_t operator + (hsh_t x, hsh_t y) {
		return hsh_t{x.v + y.v * x.l, x.l * y.l};
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;

	std::vector<int> A_orig(N); for (auto& a : A_orig) cin >> a;
	std::vector<int> B(M); for (auto& b : B) cin >> b;

	struct query_t {
		int l, r, v;
	};
	std::vector<query_t> queries(Q);

	seg_tree::in_order_layout B_layout(M);
	std::vector<int> B_seg(B_layout.N * 2);
	auto B_update_node = [&](seg_tree::point a) {
		B_seg[a] = std::gcd(B_seg[a.c(0)], B_seg[a.c(1)]);
	};
	for (int i = 0; i < M; i++) {
		B_seg[B_layout.get_point(i)] = B[i];
	}
	for (seg_tree::point a(B_layout.N - 1); a > 0; a--) B_update_node(a);
	auto B_query = [&](int lo, int hi, int v0) {
		int r = v0;
		B_layout.get_range(lo, hi).for_each_l_to_r([&](auto a) { r = std::gcd(r, B_seg[a]); });
		return r;
	};

	for (int q = 0; q < Q; q++) {
		char op; cin >> op;
		if (op == 'U') {
			int p, v; cin >> p >> v; p--;
			queries[q] = {p, -1, v};
		} else if (op == 'Q') {
			int l, r, s, t; cin >> l >> r >> s >> t; l--, s--;
			int len = B_query(s, t, r - l);
			int gap = (r - l) / len;
			queries[q] = {l, r, gap};
		} else assert(false);
	}

	std::vector<bool> ans(Q);

	std::mt19937 mt(48);
	auto random_pnum = [&]() -> pnum {
		return pnum(
			// idk, don't pick small things
			std::uniform_int_distribution(2, mnum1::MOD-1)(mt),
			std::uniform_int_distribution(2, mnum2::MOD-1)(mt)
		);
	};
	const int MAX_GAP = 500;
	{
		cerr << "CASE gap > " << MAX_GAP << endl;
		const pnum BASE = random_pnum();
		std::vector<pnum> pow_base(N+1);
		pow_base[0] = 1;
		for (int i = 0; i < N; i++) {
			pow_base[i+1] = pow_base[i] * BASE;
		}
		const pnum IBASE = pnum(1) / BASE;
		std::vector<pnum> ipow_base(N+1);
		ipow_base[0] = 1;
		for (int i = 0; i < N; i++) {
			ipow_base[i+1] = ipow_base[i] * IBASE;
		}

		std::vector<pnum> pref_pow_base(N+1);
		for (int i = 0; i < N; i++) {
			pref_pow_base[i+1] = pref_pow_base[i] + pow_base[i];
		}

		// slow update, fast query
		std::vector<pnum> A(N);

		const int K = 500;
		std::vector<pnum> bucket_prefs((N + K - 1) / K + 1);
		std::vector<pnum> prefs(N+1);
		for (int i = 0; i <= N; i++) {
			prefs[i] = (i % K == 0) ? 0 : (prefs[i-1] + A[i-1] * pow_base[i-1]);
		}
		for (int i = 0; i < N; i++) {
			bucket_prefs[i / K + 1] += A[i] * pow_base[i];
		}
		for (int i = 1; i < int(bucket_prefs.size()); i++) {
			bucket_prefs[i] += bucket_prefs[i-1];
		}

		for (int q = 0; q < Q; q++) {
			auto [l, r, v] = queries[q];
			if (r == -1) {
				// update l to v
				pnum delta = (v - std::exchange(A[l], v)) * pow_base[l];

				for (int i = l+1; i < std::min(N+1, (l / K + 1) * K); i++) {
					prefs[i] += delta;
				}
				for (int i = (l / K) + 1; i < int(bucket_prefs.size()); i++) {
					bucket_prefs[i] += delta;
				}
			} else {
				assert(l < r);
				int gap = v;
				if (gap <= MAX_GAP) continue;
				auto get_pref = [&](int i) {
					return prefs[i] + bucket_prefs[i / K];
				};
				pnum tot_sum = 0;
				pnum tgt_sum = 0;

				pnum cur_pref = get_pref(l);
				for (int i = l; i < r; i += gap) {
					pnum nxt_pref = get_pref(i + gap);
					pnum cur_val = nxt_pref - cur_pref;
					cur_pref = nxt_pref;

					tot_sum += cur_val * ipow_base[i];
					tgt_sum += pnum(A[i]);
				}
				tgt_sum *= pref_pow_base[gap];
				ans[q] = (tot_sum == tgt_sum);
			}
		}
	}

	std::vector<bool> has_gap(MAX_GAP+1);
	for (int q = 0; q < Q; q++) {
		auto [l, r, v] = queries[q];
		if (r != -1 && v <= MAX_GAP) has_gap[v] = true;
	}
	for (int gap = 1; gap <= MAX_GAP; gap++) {
		if (!has_gap[gap]) continue;
		cerr << "CASE gap = " << gap << '\n';
		std::vector<pnum> mod_vals(gap);
		{
			pnum cur_sum = 0;
			for (int i = 0; i < gap-1; i++) {
				auto v = random_pnum();
				mod_vals[i] = cur_sum;
				cur_sum += v;
			}
			mod_vals[gap-1] = -cur_sum;
		}

		// fast update, slow query
		const int K = 500;
		std::vector<pnum> A(N);
		std::vector<pnum> vals(N);
		std::vector<pnum> bucket_sums((N + K - 1) / K);
		for (int i = 0; i < N; i++) {
			A[i] = A_orig[i];
			vals[i] = A[i] * mod_vals[i % gap];
			bucket_sums[i/K] += vals[i];
		}

		for (int q = 0; q < Q; q++) {
			auto [l, r, v] = queries[q];
			if (r == -1) {
				// update l to v
				pnum delta = v - std::exchange(A[l], v);
				vals[v] += delta * mod_vals[l % gap];
				bucket_sums[l / K] += delta * mod_vals[l % gap];
			} else {
				assert(l < r);
				if (v != gap) continue;
				// query l to r
				int dists = std::min(l % K, K - l % K) + std::min({r % K, K - r % K, N - r});
				pnum sum = 0;
				if (r - l <= dists) {
					sum = std::accumulate(vals.begin() + l, vals.begin() + r, pnum(0));
				} else {
					if (r % K <= std::min(K - r % K, N - r)) {
						// move downwards
						sum += std::accumulate(vals.begin() + r - r % K, vals.begin() + r, pnum(0));
						r -= r % K;
						r /= K;
					} else {
						sum -= std::accumulate(vals.begin() + r, vals.begin() + std::min(N, r + (K - r % K)), pnum(0));
						r += K - r % K;
						r /= K;
					}
					if (l % K <= K - l % K) {
						sum -= std::accumulate(vals.begin() + l - l % K, vals.begin() + l, pnum(0));
						l -= l % K;
						l /= K;
					} else {
						sum += std::accumulate(vals.begin() + l, vals.begin() + l + (K - l % K), pnum(0));
						l += K - l % K;
						l /= K;
					}
					assert(l <= r);
					sum = std::accumulate(bucket_sums.begin() + l, bucket_sums.begin() + r, sum);
				}
				cerr << sum << '\n';
				ans[q] = (sum == 0);
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		if (queries[q].r != -1) {
			cout << (ans[q] ? "Yes" : "No") << '\n';
		}
	}

	return 0;
}
