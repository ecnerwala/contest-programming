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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	assert(Q > 0);
	struct op_t {
		int64_t d;
		int64_t b;
		int q_st;
		int q_en;
	};
	std::vector<op_t> ops(Q);
	for (int q = 0; q < Q; q++) {
		cin >> ops[q].d >> ops[q].b;
		ops[q].q_st = q;
	}

	std::sort(ops.begin(), ops.end(), [&](const auto& a, const auto& b) {
		return std::tie(a.d, a.q_st) < std::tie(b.d, b.q_st);
	});

	for (int i = 0; i < int(ops.size()); i++) {
		if (i+1 < int(ops.size()) && ops[i].d == ops[i+1].d) {
			ops[i].q_en = ops[i+1].q_st;
		} else {
			ops[i].q_en = Q;
		}
	}

	using num = modnum<int(1e9)+7>;
	struct seg_t {
		int num_marks = 0;
		int64_t cnt_free = 0;
		num sum_taken = 0;
		num sum_whole = 0;
	};

	std::vector<int64_t> cur_extra(Q);
	std::vector<int64_t> days(Q+1);
	for (int i = 0; i < Q; i++) days[i] = ops[i].d;
	const int64_t INF = 1e18;
	days[Q] = INF;

	seg_tree::in_order_layout layout(Q);
	std::vector<seg_t> seg(2*Q);
	auto update_node = [&](seg_tree::point a) -> void {
		assert(seg[a].num_marks >= 0);
		if (seg[a].num_marks) {
			seg[a].cnt_free = 0;
			seg[a].sum_taken = seg[a].sum_whole;
		} else {
			if (a >= Q) {
				int i = layout.get_leaf_index(a);
				seg[a].cnt_free = (days[i+1] - days[i] - cur_extra[i]);
				assert(seg[a].cnt_free >= 0);
				seg[a].sum_taken = num(cur_extra[i]) * num(days[i] * 2 + cur_extra[i] - 1) / num(2);
			} else {
				seg[a].cnt_free = seg[a.c(0)].cnt_free + seg[a.c(1)].cnt_free;
				seg[a].sum_taken = seg[a.c(0)].sum_taken + seg[a.c(1)].sum_taken;
			}
		}
	};
	for (seg_tree::point a(2*Q-1); a >= Q; a--) {
		int i = layout.get_leaf_index(a);
		seg[a].sum_whole = num(days[i+1] - days[i]) * num(days[i] + days[i+1] - 1) / num(2);
		update_node(a);
	}
	for (seg_tree::point a(Q-1); a > 0; a--) {
		seg[a].sum_whole = seg[a.c(0)].sum_whole + seg[a.c(1)].sum_whole;
		update_node(a);
	}

	std::vector<num> ans(Q);

	std::vector<int> root_ops; root_ops.reserve(Q);
	for (int o = 0; o < Q; o++) {
		if (ops[o].b == 0) continue;
		root_ops.push_back(o);
		//cerr << "op " << o << ": " << ops[o].d << ' ' << ops[o].b << " from " << ops[o].q_st << '-' << ops[o].q_en << '\n';
	}
	std::y_combinator([&](auto self, int q_lo, int q_hi, std::vector<int> range_ops) -> void {
		struct cur_op_t {
			int o;
			int r;
			int64_t extra;
		};
		int num_cur = 0;
		for (auto o : range_ops) {
			if (ops[o].q_st <= q_lo && q_hi <= ops[o].q_en) {
				num_cur++;
			}
		}
		std::vector<cur_op_t> cur_ops; cur_ops.reserve(num_cur);
		for (auto o : range_ops) {
			if (ops[o].q_st <= q_lo && q_hi <= ops[o].q_en) {
				//cerr << "performing op " << o << '\n';
				// perform this op
				int r = -1;
				int64_t b = ops[o].b;
				int64_t extra = 0;
				std::y_combinator([&](auto rec, seg_tree::point a) -> void {
					if (b == 0) return;
					if (seg[a].num_marks > 0) return;
					auto bounds = layout.get_node_bounds(a);
					if (bounds[1] <= o) return;
					if (bounds[0] < o) {
						rec(a.c(0));
						rec(a.c(1));
						return;
					}
					if (b > seg[a].cnt_free) {
						// eat the entirety
						b -= seg[a].cnt_free;
						return;
					}
					if (a >= Q) {
						r = layout.get_leaf_index(a);
						extra = b;
						b = 0;
						return;
					}

					rec(a.c(0));
					rec(a.c(1));
				})(seg_tree::point(1));
				assert(r != -1);
				cur_ops.push_back({o, r, extra});
				{
					auto rng = layout.get_range(o, r);
					rng.for_each([&](seg_tree::point a) -> void {
						seg[a].num_marks++;
						update_node(a);
					});
					rng.for_parents_up(update_node);
				}
				{
					auto a = layout.get_point(r);
					cur_extra[r] += extra;
					update_node(a);
					a.for_parents_up(update_node);
				}
			}
		}
		if (q_hi - q_lo == 1) {
			ans[q_lo] = seg[1].sum_taken;
		} else {
			int q_mi = q_lo + (q_hi - q_lo) / 2;
			std::vector<int> left_ops; left_ops.reserve(range_ops.size());
			std::vector<int> right_ops; right_ops.reserve(range_ops.size());
			for (auto o : range_ops) {
				if (ops[o].q_st <= q_lo && q_hi <= ops[o].q_en) {
					continue;
				}
				if (ops[o].q_st < q_mi) {
					left_ops.push_back(o);
				}
				if (ops[o].q_en > q_mi) {
					right_ops.push_back(o);
				}
			}
			range_ops = {};
			self(q_lo, q_mi, std::move(left_ops));
			self(q_mi, q_hi, std::move(right_ops));
		}
		std::reverse(cur_ops.begin(), cur_ops.end());
		for (auto [o, r, extra] : cur_ops) {
			//cerr << "undoing op " << o << '\n';
			{
				auto rng = layout.get_range(o, r);
				rng.for_each([&](seg_tree::point a) -> void {
					seg[a].num_marks--;
					update_node(a);
				});
				rng.for_parents_up(update_node);
			}
			{
				auto a = layout.get_point(r);
				cur_extra[r] -= extra;
				update_node(a);
				a.for_parents_up(update_node);
			}
		}
	})(0, Q, root_ops);

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
