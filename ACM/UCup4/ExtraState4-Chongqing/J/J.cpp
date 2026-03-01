#include <bits/stdc++.h>

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

	int64_t N; int M, Q; std::cin >> N >> M >> Q;
	struct op_t {
		int64_t l, r; int delta;
	};
	std::vector<op_t> ops;
	auto queue_update_range = [&](int64_t l, int64_t r, int delta) -> void {
		ops.push_back({l, r, delta});
	};
	auto queue_print_ans = [&]() -> void {
		ops.push_back({-1, -1, 0});
	};
	auto update_loc = [&](int64_t l, int64_t r, int delta) -> void {
		//std::cerr << "update_loc " << l << ' ' << r << ' ' << delta << '\n';
		assert(l < r);
		if (r - l >= N) return;
		// now, everything that would put l and r together is bad
		{
			int64_t e = l / N * N;
			l -= e, r -= e;
		}
		assert(l < N);
		assert(r < 2*N);
		if (r < N) {
			assert(0 < N-r);
			queue_update_range(0, N-r, delta);
			if (l > 0) {
				queue_update_range(N-l, N, delta);
			}
		} else {
			assert(N-l < 2*N-r);
			queue_update_range(N-l, 2*N-r, delta);
		}
	};
	std::map<int64_t, std::set<int>> locs;
	auto update_set = [&](std::set<int>& s, std::set<int>::iterator it, int delta) -> void {
		if (it != s.begin() && std::next(it) != s.end()) {
			update_loc(*std::prev(it), *std::next(it), -delta);
		}
		if (it != s.begin()) {
			update_loc(*std::prev(it), *it, delta);
		}
		if (std::next(it) != s.end()) {
			update_loc(*it, *std::next(it), delta);
		}
	};
	std::vector<int64_t> F(M, -1);
	auto set_f = [&](int i, int64_t new_val) -> void {
		if (F[i] != -1) {
			auto& s = locs[F[i]];
			auto it = s.find(i);
			assert(it != s.end());
			update_set(s, it, -1);

			s.erase(it);
		}
		F[i] = new_val;
		if (F[i] != -1) {
			auto& s = locs[F[i]];
			auto [it, did_insert] = s.insert(i);
			assert(did_insert);
			update_set(s, it, +1);
		}
	};
	for (int i = 0; i < M; i++) {
		int64_t f; std::cin >> f; f--;
		set_f(i, f);
	}

	queue_print_ans();
	for (int q = 0; q < Q; q++) {
		int i; int64_t f; std::cin >> i >> f; i--, f--;
		set_f(i, f);
		queue_print_ans();
	}

	std::vector<int64_t> bounds; bounds.reserve(2 + ops.size() * 2);
	bounds.push_back(0);
	bounds.push_back(N);
	for (auto op : ops) {
		//std::cerr << op.l << ' ' << op.r << ' ' << op.delta << '\n';
		if (op.l == -1 && op.r == -1 && op.delta == 0) {
			// do nothing
		} else {
			assert(op.l < op.r && std::abs(op.delta) == 1);
			bounds.push_back(op.l);
			bounds.push_back(op.r);
		}
	}
	std::sort(bounds.begin(), bounds.end());
	bounds.resize(std::unique(bounds.begin(), bounds.end()) - bounds.begin());

	std::vector<int64_t> cnts(int(bounds.size()) - 1);
	std::vector<int64_t> tot_xors(int(bounds.size()) - 1);
	for (int i = 0; i < int(bounds.size()); i++) {
		int64_t v = bounds[i];
		int64_t cur_xor = ((v & 1) ? (v-1) : 0) ^ ((v & 2) ? 1 : 0);
		if (i < int(bounds.size()) - 1) cnts[i] -= v, tot_xors[i] ^= cur_xor;
		if (i) cnts[i-1] += v, tot_xors[i-1] ^= cur_xor;
	}

	seg_tree::in_order_layout layout(int(bounds.size())-1);
	struct seg_t {
		int lazy_bad;
		int64_t cnt = 0;
		int64_t tot_xor = 0;
	};
	std::vector<seg_t> seg(2*layout.N);
	auto update_node = [&](seg_tree::point a) -> void {
		assert(seg[a].lazy_bad >= 0);
		if (seg[a].lazy_bad) {
			seg[a].cnt = 0;
			seg[a].tot_xor = 0;
		} else if (a < layout.N) {
			seg[a].cnt = seg[a.c(0)].cnt + seg[a.c(1)].cnt;
			seg[a].tot_xor = seg[a.c(0)].tot_xor ^ seg[a.c(1)].tot_xor;
		} else {
			int i = layout.get_leaf_index(a);
			seg[a].cnt = cnts[i];
			seg[a].tot_xor = tot_xors[i];
		}
	};
	for (seg_tree::point a(2*layout.N-1); a > 0; a--) update_node(a);

	auto update_range = [&](int64_t l, int64_t r, int delta) -> void {
		int lo = int(std::lower_bound(bounds.begin(), bounds.end(), l) - bounds.begin());
		int hi = int(std::lower_bound(bounds.begin(), bounds.end(), r) - bounds.begin());
		auto rng = layout.get_range(lo, hi);
		rng.for_each([&](seg_tree::point a) -> void {
			seg[a].lazy_bad += delta;
			update_node(a);
		});
		rng.for_parents_up(update_node);
	};

	auto print_ans = [&]() -> void {
		std::cout << seg[1].cnt << ' ' << seg[1].tot_xor << '\n';
	};

	for (auto op : ops) {
		if (op.l == -1 && op.r == -1 && op.delta == 0) {
			print_ans();
		} else {
			assert(op.l < op.r && std::abs(op.delta) == 1);
			update_range(op.l, op.r, op.delta);
		}
	}

	return 0;
}
