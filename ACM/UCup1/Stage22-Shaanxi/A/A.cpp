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

struct op_t {
	int state = 0;
	int up0 = 0;
	int down0 = 0;
	int up1 = 0;
	int down1 = 0;
	int down2 = 0;

	void do_down(int v) {
		if (state == 0) {
			down0 += v;
		} else if (state == 1) {
			down1 += v;
		} else if (state == 2) {
			down1 += v;
		} else if (state == 3) {
			down2 += v;
		} else assert(false);
	}
	void do_up(int v) {
		if (state == 0) {
			down0 -= v;
			if (down0 < 0) {
				up0 -= down0;
				down0 = 0;
			}
		} else if (state == 1) {
			down1 -= v;
			if (down1 < 0) {
				state = 0;
				down0 += down1;
				down1 = 0;
				if (down0 < 0) {
					up0 -= down0;
					down0 = 0;
				}
			}
		} else if (state == 2) {
			down1 -= v;
			if (down1 < 0) {
				up1 -= down1;
				down1 = 0;
			}
		} else if (state == 3) {
			down2 -= v;
			if (down2 < 0) {
				state = 2;
				down1 += down2;
				down2 = 0;
				if (down1 < 0) {
					up1 -= down1;
					down1 = 0;
				}
			}
		} else assert(false);
	}
	void do_checkpoint() {
		if (state == 0) {
			state = 1;
			down1 = 0;
		} else if (state == 1) {
			// do nothing
		} else if (state == 2) {
			state = 3;
			down2 = 0;
		} else if (state == 3) {
			// do nothing
		} else assert(false);
	}
	void do_restore() {
		if (state == 0) {
			up1 = down1 = 0;
			state = 2;
		} else if (state == 1) {
			up1 = down1 = 0;
			state = 2;
		} else if (state == 2) {
			// noop
		} else if (state == 3) {
			state = 2;
			down2 = 0;
		}
	}

	friend op_t merge(const op_t& a, const op_t& b) {
		op_t r = a;
		if (b.state == 0) {
			r.do_up(b.up0);
			r.do_down(b.down0);
		} else if (b.state == 1) {
			r.do_up(b.up0);
			r.do_down(b.down0);
			r.do_checkpoint();
			r.do_down(b.down1);
		} else if (b.state == 2) {
			r.do_up(b.up0);
			r.do_down(b.down0);
			r.do_restore();
			r.do_up(b.up1);
			r.do_down(b.down1);
		} else if (b.state == 3) {
			r.do_up(b.up0);
			r.do_down(b.down0);
			r.do_restore();
			r.do_up(b.up1);
			r.do_down(b.down1);
			r.do_checkpoint();
			r.do_down(b.down2);
		} else assert(false);
		return r;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	std::vector<op_t> ops(Q);

	std::vector<bool> is_ans(Q);
	std::vector<int> ans(Q);

	std::vector<std::vector<int>> starts(N);
	std::vector<std::vector<int>> queries(N);
	std::vector<std::vector<int>> ends(N);

	for (int q = 0; q < Q; q++) {
		int op, l, r; cin >> op >> l >> r; l--, r--;
		if (op == 1) {
			ops[q].do_down(1);
		} else if (op == 2) {
			ops[q].do_up(1);
		} else if (op == 3) {
			ops[q].do_checkpoint();
		} else if (op == 4) {
			ops[q].do_restore();
		} else if (op == 5) {
			is_ans[q] = true;
		} else assert(false);

		if (is_ans[q]) {
			assert(l == r);
			queries[l].push_back(q);
		} else {
			starts[l].push_back(q);
			ends[r].push_back(q);
		}
	}

	seg_tree::in_order_layout layout(Q);
	std::vector<op_t> seg(2*Q);
	auto update_node = [&](seg_tree::point a) -> void {
		seg[a] = merge(seg[a.c(0)], seg[a.c(1)]);
	};
	for (int i = 0; i < N; i++) {
		for (int q : starts[i]) {
			auto a = layout.get_point(q);
			seg[a] = ops[q];
			a.for_parents_up(update_node);
		}
		for (int q : queries[i]) {
			auto rng = layout.get_range(0, q);
			op_t res;
			rng.for_each_l_to_r([&](auto a) -> void {
				res = merge(res, seg[a]);
			});
			ans[q] = int(5e5) - res.up0 + res.down0 - res.up1 + res.down1 + res.down2;
		}
		for (int q : ends[i]) {
			auto a = layout.get_point(q);
			seg[a] = op_t{};
			a.for_parents_up(update_node);
		}
	}
	for (int q = 0; q < Q; q++) {
		if (is_ans[q]) cout << ans[q] << '\n';
	}

	return 0;
}
