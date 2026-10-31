#include <bits/stdc++.h>
#include <cassert>

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
inline int floor_log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int ceil_log_2(int a) {
	return a ? floor_log_2(2*a-1) : -1;
}

inline int next_pow_2(int a) {
	return 1 << ceil_log_2(a);
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

	template <typename F> void for_each_down(F f) const {
		// strictly greater than 0
		for (int L = floor_log_2(a); L >= 0; L--) {
			f(point(a >> L));
		}
	}

	template <typename F> void for_each_up(F f) const {
		for (int v = a; v > 0; v >>= 1) {
			f(point(v));
		}
	}

	template <typename F> void for_parents_down(F f) const {
		// strictly greater than 0
		for (int L = floor_log_2(a); L > 0; L--) {
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
		int anc_depth = floor_log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = (-a) & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point(((a-1) >> i) + 1));
		}
		for (int v = b & anc_msk; v; ) {
			int i = floor_log_2(v);
			f(point((b >> i) - 1));
			v ^= (1 << i);
		}
	}

	// Iterate over the range from right to left.
	//    Calls f(point)
	template <typename F> void for_each_r_to_l(F f) const {
		int anc_depth = floor_log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = b & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point((b >> i) - 1));
		}
		for (int v = (-a) & anc_msk; v; ) {
			int i = floor_log_2(v);
			f(point(((a-1) >> i) + 1));
			v ^= (1 << i);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = floor_log_2((x-1) ^ y);
		for (int i = floor_log_2(x); i > dx; i--) {
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
		int anc_depth = floor_log_2((x-1) ^ y);
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
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q; std::cin >> N >> Q;

		struct arr_val_t {
			// all required to be nonempty
			bool has = false;
			int64_t val = 0;
			int64_t tot = 0;
			int64_t pref = 0;
			int64_t suff = 0;
		};
		auto make_arr_val_t = [&](int64_t v) -> arr_val_t {
			return {true, v, v, v, v};
		};
		auto concat = [&](arr_val_t a, arr_val_t b) -> arr_val_t {
			if (!a.has) return b;
			if (!b.has) return a;
			return {
				true,
				std::max({a.val, b.val, a.suff + b.pref}),
				a.tot + b.tot,
				std::max(a.pref, a.tot + b.pref),
				std::max(b.suff, b.tot + a.suff)
			};
		};
		auto scale = [&](arr_val_t a, int64_t s) -> arr_val_t {
			a.val *= s;
			a.tot *= s;
			a.pref *= s;
			a.suff *= s;
			return a;
		};

		struct op_t {
			// let's start by maintaining the current value
			// first, it could be set
			bool has_set = false;
			int set_val = 0; // stores the current value
			// otherwise, for each input signum, we have an output signum
			std::array<int, 3> new_sign = {-1, 0, +1};

			std::array<arr_val_t, 3> pref_vals = {arr_val_t{}, arr_val_t{}, arr_val_t{}}; // this is just in terms of the input
			// tail_val only valid if has_set = true
			arr_val_t tail_val = arr_val_t{};
		};
		auto signum = [&](int v) -> int { return (v > 0) - (v < 0); };
		// first do a, then b
		auto apply_op = [&](op_t& a, const op_t& b) -> void {
			// first, apply b.pref_vals
			if (a.has_set) {
				int z2 = signum(a.set_val);
				a.tail_val = concat(a.tail_val, scale(b.pref_vals[z2+1], std::abs(a.set_val)));
				a.set_val = std::abs(a.set_val) * b.new_sign[z2+1];
			} else {
				for (int z = -1; z <= 1; z++) {
					int z2 = a.new_sign[z+1];
					a.new_sign[z+1] = b.new_sign[z2+1];
					a.pref_vals[z+1] = concat(a.pref_vals[z+1], b.pref_vals[z2+1]);
				}
			}
			if (b.has_set) {
				// now, apply b.tail_val
				if (!a.has_set) {
					a.tail_val = arr_val_t{};
					a.has_set = true;
				}
				a.set_val = b.set_val;
				a.tail_val = concat(a.tail_val, b.tail_val);
			}
		};

		seg_tree::in_order_layout layout(N);
		std::vector<op_t> seg(layout.N*2, op_t{});
		auto downdate_node = [&](seg_tree::point a) -> void {
			apply_op(seg[a.c(0)], seg[a]);
			apply_op(seg[a.c(1)], seg[a]);
			seg[a] = op_t{};
		};
		for (int i = 0; i < N; i++) {
			int v; std::cin >> v;
			seg_tree::point a = layout.get_point(i);
			seg[a].has_set = true;
			seg[a].set_val = v;
			seg[a].tail_val = make_arr_val_t(v);
		}
		uint64_t last_ans = 0;
		for (int q = 0; q < Q; q++) {
			int op; std::cin >> op;
			auto read_enc = [&]() -> int {
				uint64_t u; std::cin >> u;
				u ^= last_ans;
				return int(u % N);
			};
			auto read_range = [&]() -> auto {
				int l = read_enc();
				int r = read_enc();
				if (l > r) std::swap(l, r);
				return layout.get_range(l, r+1);
			};
			if (op == 1) {
				auto rng = read_range();
				int x; std::cin >> x;
				rng.for_parents_down(downdate_node);
				op_t cur;
				cur.has_set = true;
				cur.set_val = x;
				rng.for_each([&](seg_tree::point a) -> void { apply_op(seg[a], cur); });
			} else if (op == 2) {
				auto rng = read_range();
				rng.for_parents_down(downdate_node);
				op_t cur;
				cur.new_sign = {+1, 0, -1};
				rng.for_each([&](seg_tree::point a) -> void { apply_op(seg[a], cur); });
			} else if (op == 3) {
				auto rng = read_range();
				rng.for_parents_down(downdate_node);
				op_t cur;
				cur.new_sign = {0, 0, +1};
				rng.for_each([&](seg_tree::point a) -> void { apply_op(seg[a], cur); });
			} else if (op == 4) {
				auto a = layout.get_point(read_enc());
				a.for_parents_down(downdate_node);
				int64_t ans = seg[a].tail_val.val;
				std::cout << ans << '\n';
				last_ans = uint64_t(ans);
			} else assert(false);
			{
				op_t adv;
				adv.pref_vals[0] = make_arr_val_t(adv.new_sign[0]);
				adv.pref_vals[1] = make_arr_val_t(adv.new_sign[1]);
				adv.pref_vals[2] = make_arr_val_t(adv.new_sign[2]);
				apply_op(seg[1], adv);
			}
		}
	}
	return 0;
}
