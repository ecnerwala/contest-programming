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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	int64_t cur_time = 0;

	seg_tree::in_order_layout layout(N);

	const int64_t INF = 1e18;

	struct small_seg_t {
		int64_t sum_val;
		int64_t sum_idx;

		int64_t max_idx;
		int64_t max_val;
		int64_t next_evt;
	};
	std::vector<small_seg_t> small_seg(2*N);
	auto update_small_node = [&](seg_tree::point a) {
		auto& n = small_seg[a];
		if (a >= N) {
			n.max_idx = n.sum_idx;
			n.max_val = n.sum_val;
			n.next_evt = INF;
			return;
		}
		assert(a < N);
		auto& c0 = small_seg[a.c(0)];
		auto& c1 = small_seg[a.c(1)];
		n.sum_idx = c0.sum_idx + c1.sum_idx;
		n.sum_val = c0.sum_val + c1.sum_val;

		if (c0.max_idx == 0) {
			n.max_idx = c1.max_idx;
			n.max_val = c1.max_val;
			n.next_evt = INF;
		} else if (c1.max_idx == 0) {
			n.max_idx = c0.max_idx;
			n.max_val = c0.max_val;
			n.next_evt = INF;
		} else {
			assert(c0.max_idx < c1.max_idx);
			if (c0.max_val + c0.max_idx * cur_time > c1.max_val + c1.max_idx * cur_time) {
				n.max_idx = c0.max_idx;
				n.max_val = c0.max_val;
				int64_t numer = c0.max_val - c1.max_val;
				int64_t denom = c1.max_idx - c0.max_idx;
				assert(numer > 0);
				assert(denom > 0);
				n.next_evt = (numer + denom - 1) / denom;
			} else {
				n.max_idx = c1.max_idx;
				n.max_val = c1.max_val;
				n.next_evt = INF;
			}
		}
		n.next_evt = std::min(n.next_evt, c0.next_evt);
		n.next_evt = std::min(n.next_evt, c1.next_evt);
	};
	for (int i = 0; i < N; i++) {
		seg_tree::point a = layout.get_point(i);
		small_seg[a].sum_idx = i+1;
		small_seg[a].sum_val = A[i];
		update_small_node(a);
	}
	for (seg_tree::point a(N-1); a > 0; a--) {
		update_small_node(a);
	}

	auto heaten_small = [&]() -> void {
		cur_time++;
		std::y_combinator([&](auto self, seg_tree::point a) -> void {
			assert(a < 2*N);
			if (cur_time < small_seg[a].next_evt) {
				return;
			}
			self(a.c(0));
			self(a.c(1));
			update_small_node(a);
		})(seg_tree::point(1));
	};

	auto sum_small = [&](seg_tree::range rng) -> int64_t {
		int64_t ans = 0;
		rng.for_each([&](seg_tree::point a) -> void {
			ans += small_seg[a].sum_val + small_seg[a].sum_idx * cur_time;
		});
		return ans;
	};

	struct big_seg_t {
		int64_t sum_val;
		int64_t sum_idx;
		int64_t sum_cnt;

		// always the rightmost
		int64_t max_val;
		int64_t max_idx;

		// set then add
		int64_t lazy_set = -1;
		int64_t lazy_add = 0;
	};
	std::vector<big_seg_t> big_seg(2*N);
	auto update_big_node = [&](seg_tree::point a) {
		auto& n = big_seg[a];
		assert(n.lazy_set == -1);
		assert(n.lazy_add == 0);

		if (a >= N) {
			n.max_val = n.sum_val;
			n.max_idx = n.sum_idx;
			return;
		}

		assert(a < N);
		auto& c0 = big_seg[a.c(0)];
		auto& c1 = big_seg[a.c(1)];
		n.sum_idx = c0.sum_idx + c1.sum_idx;
		n.sum_val = c0.sum_val + c1.sum_val;
		n.sum_cnt = c0.sum_cnt + c1.sum_cnt;

		if (c1.max_idx == 0) {
			n.max_idx = c0.max_idx;
			n.max_val = c0.max_val;
		} else {
			n.max_idx = c1.max_idx;
			n.max_val = c1.max_val;
		}
	};
	auto apply_big_set = [&](seg_tree::point a, int64_t set_val) {
		assert(set_val != -1);
		auto& n = big_seg[a];
		n.sum_val = set_val * n.sum_cnt;
		n.max_val = set_val;
		n.lazy_add = 0;
		n.lazy_set = set_val;
	};
	auto apply_big_add = [&](seg_tree::point a, int64_t add_val) {
		auto& n = big_seg[a];
		n.sum_val += n.sum_idx * add_val;
		n.max_val += n.max_idx * add_val;
		n.lazy_add += add_val;
	};
	auto downdate_big_node = [&](seg_tree::point a) {
		auto& n = big_seg[a];
		if (a >= N) {
			n.lazy_set = -1;
			n.lazy_add = 0;
			return;
		}
		if (n.lazy_set != -1) {
			apply_big_set(a.c(0), n.lazy_set);
			apply_big_set(a.c(1), n.lazy_set);
			n.lazy_set = -1;
		}
		if (n.lazy_add != 0) {
			apply_big_add(a.c(0), n.lazy_add);
			apply_big_add(a.c(1), n.lazy_add);
			n.lazy_add = 0;
		}
	};
	for (int i = 0; i < N; i++) {
		seg_tree::point a = layout.get_point(i);
		big_seg[a].sum_cnt = 0;
		big_seg[a].sum_idx = 0;
		big_seg[a].sum_val = 0;
		update_big_node(a);
	}
	for (seg_tree::point a(N-1); a > 0; a--) {
		update_big_node(a);
	}
	auto heaten_big = [&]() -> void {
		apply_big_add(seg_tree::point(1), 1);
	};
	auto sum_big = [&](seg_tree::range rng) -> int64_t {
		int64_t ans = 0;
		rng.for_parents_down(downdate_big_node);
		rng.for_each([&](seg_tree::point a) -> void {
			ans += big_seg[a].sum_val;
		});
		return ans;
	};

	auto do_min_big = [&](int64_t v) -> void {
		seg_tree::point a(1);
		if (big_seg[a].max_idx == 0) {
			return;
		}
		if (big_seg[a].max_val < v) {
			return;
		}
		while (true) {
			assert(big_seg[a].max_idx != 0);
			assert(big_seg[a].max_val >= v);
			downdate_big_node(a);
			if (a >= N) {
				apply_big_set(a, v);
				a.for_parents_up(update_big_node);
				break;
			}
			if (big_seg[a.c(0)].max_idx != 0 && big_seg[a.c(0)].max_val >= v) {
				assert(big_seg[a.c(1)].max_idx == 0 || big_seg[a.c(1)].max_val >= v);
				apply_big_set(a.c(1), v);
				a = a.c(0);
			} else {
				assert(big_seg[a.c(1)].max_idx != 0);
				assert(big_seg[a.c(1)].max_val >= v);
				a = a.c(1);
			}
		}
	};

	auto do_min_small = [&](int64_t v) -> void {
		while (small_seg[1].max_idx != 0 && small_seg[1].max_val + small_seg[1].max_idx * cur_time >= v) {
			int i = int(small_seg[1].max_idx - 1);
			seg_tree::point a = layout.get_point(i);
			small_seg[a].sum_idx = 0;
			small_seg[a].sum_val = 0;
			update_small_node(a);
			a.for_parents_up(update_small_node);

			a.for_parents_down(downdate_big_node);
			downdate_big_node(a);
			big_seg[a].sum_idx = i+1;
			big_seg[a].sum_val = v;
			big_seg[a].sum_cnt = 1;
			update_big_node(a);
			a.for_parents_up(update_big_node);
		}
	};

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			// do the actual stuff
			int64_t v; cin >> v;
			do_min_big(v);
			do_min_small(v);
		} else if (op == 2) {
			heaten_big();
			heaten_small();
		} else if (op == 3) {
			int l, r; cin >> l >> r; l--;
			auto rng = layout.get_range(l, r);
			cout << sum_big(rng) + sum_small(rng) << '\n';
		} else assert(false);
	}

	return 0;
}
