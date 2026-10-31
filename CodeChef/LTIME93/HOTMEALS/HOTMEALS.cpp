#include <bits/stdc++.h>

void wssert(bool b) { if (!b) { std::cout << "GARBAGE" << '\n'; exit(0); }}

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
	explicit point(int a_) : a(a_) { assert(a >= 0); }

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
	range() : a(0), b(0) {}
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
	//   Calls f(point a, bool is_right)
	template <typename F> void for_each(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++), false);
			if (y & 1) f(point(--y), true);
		}
	}

	template <typename F> void for_each_l_to_r(F f) const {
		assert(a && b);
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

	template <typename F> void for_each_r_to_l(F f) const {
		assert(a && b);
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

	// Useful when you want to traverse the tree without calling downdate
	template <typename F, typename PushFunc, typename PopFunc> void euler_tour_l_to_r(F f, PushFunc push, PopFunc pop) const {
		// TODO
	}

	// Useful when you want to traverse the tree without calling downdate
	template <typename F, typename PushFunc, typename PopFunc> void euler_tour_r_to_l(F f, PushFunc push, PopFunc pop) const {
		// TODO
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
		auto [x, y] = get_node_bounds(pt);
		return y - x;
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
		auto [x, y] = get_node_bounds(pt);
		int r = y - x;
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

struct seg_node {
	int64_t max_val = 0;
	int64_t lazy = 0;
	int left_big = 0;
	int cnt_big = 0;
};

seg_tree::in_order_layout layout;
std::vector<seg_node> seg;

void apply_increment(seg_tree::point a, int64_t v) {
	seg[a].max_val += v;
	seg[a].lazy += v;
}

void downdate_node(seg_tree::point a) {
	if (a >= layout.N) return;
	if (seg[a].lazy) {
		apply_increment(a[0], seg[a].lazy);
		apply_increment(a[1], seg[a].lazy);
		seg[a].lazy = 0;
	}
}

int get_bigger(seg_tree::point cur, int64_t v) {
	int ans = 0;
	if (seg[cur].max_val > v) {
		while (true) {
			wssert(seg[cur].max_val > v);
			if (cur >= layout.N) {
				ans++;
				break;
			}
			downdate_node(cur);
			assert(seg[cur].max_val == std::max(seg[cur[0]].max_val, seg[cur[1]].max_val));
			if (seg[cur[1]].max_val > v) {
				ans += seg[cur].left_big;
				cur = cur[1];
			} else {
				cur = cur[0];
			}
		}
	}
	return ans;
}

void update_node(seg_tree::point a) {
	if (a >= layout.N) {
		seg[a].cnt_big = 1;
		return;
	}

	wssert(a < layout.N);
	seg[a].max_val = std::max(seg[a[1]].max_val, seg[a[0]].max_val);
	seg[a].left_big = get_bigger(a[0], seg[a[1]].max_val);
	seg[a].cnt_big = seg[a].left_big + seg[a[1]].cnt_big;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;

	for (int i = 0; i < N; i++) {
		A[i] -= i;
	}

	layout = seg_tree::in_order_layout(N);
	seg.resize(2*N);
	for (int i = 0; i < N; i++) {
		seg[layout.get_point(i)].max_val = A[i];
	}
	for (int a = 2*N-1; a > 0; a--) {
		update_node(seg_tree::point(a));
	}

	auto do_query = [&](int l, int r) -> int {
		auto rng = layout.get_range(l, r);
		rng.for_parents_down(downdate_node);
		int64_t cur_max = -1e18;
		int ans = 0;
		rng.for_each_r_to_l([&](seg_tree::point a) {
			ans += get_bigger(a, cur_max);
			cur_max = max(cur_max, seg[a].max_val);
		});
		return r - l - ans;
	};

	cout << do_query(0, N) << '\n';

	int Q; cin >> Q;
	while (Q--) {
		int op; cin >> op;
		if (op == 1) {
			int l, r; cin >> l >> r; l--;
			cout << do_query(l, r) << '\n';
		} else if (op == 2) {
			int l, r; cin >> l >> r; l--;
			int w; cin >> w;

			auto rng = layout.get_range(l, r);
			rng.for_parents_down(downdate_node);
			rng.for_each([&](seg_tree::point p, bool) {
				apply_increment(p, w);
			});
			vector<int> parents;
			rng.for_parents_up(update_node);
		} else assert(false);
	}

	return 0;
}

// An interval says how much help it needs and how many it produces
// Merging 2 intervals: {help1, make1}, {help2, make2} -> {help1 + max(help2 - make1, 0), }
