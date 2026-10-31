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

	// Let's count the total winding number, tracing each region CW.
	// Each intersection adds 2 to the white and black winding numbers.
	// Each black (interior) corner adds 1 to the black winding number and -1 to the white.

	// The total number of regions is the total winding number divided by 4,
	// except the white gets +1 + # cc's because the outer face has winding number -# cc's.
	
	int N, T; cin >> N >> T;
	std::vector<std::array<std::array<int, 2>, 2>> rects(N);
	for (auto& r : rects) {
		int xlo, ylo, xhi, yhi; cin >> xlo >> ylo >> xhi >> yhi;
		xlo--, ylo--, xhi--, yhi--;
		r[0] = {xlo, xhi};
		r[1] = {ylo, yhi};
	}

	const int X = 2*N;

	std::vector<std::pair<int, std::array<int, 2>>> x_evts(X);
	for (int i = 0; i < N; i++) {
		x_evts[rects[i][0][0]] = {2*i+0, rects[i][1]};
		x_evts[rects[i][0][1]] = {2*i+1, rects[i][1]};
	}

	int64_t num_inter = 0;
	int vert_black_minus_white = 0;

	std::vector<int> rect_color(X);
	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	seg_tree::in_order_layout layout(X);
	// id if there's exactly 1, else -1 if empty else -2
	std::vector<int> seg_cc(2*X, -1);
	std::vector<int> seg_cnt(2*X, 0);
	auto update_node = [&](seg_tree::point a) {
		assert(a < X);
		int x = seg_cc[a.c(0)];
		int y = seg_cc[a.c(1)];
		if (x == -2 || y == -2) {
			seg_cc[a] = -2;
			return;
		}
		if (x == -1) {
			seg_cc[a] = y;
			return;
		}
		if (y == -1) {
			seg_cc[a] = x;
			return;
		}
		x = get_par(x);
		y = get_par(y);
		if (x == y) {
			seg_cc[a] = x;
			return;
		}

		seg_cc[a] = -2;
	};

	auto connect_node = std::y_combinator([&](auto self, seg_tree::point a, int conn) -> void {
		if (seg_cc[a] == -1) return;
		if (seg_cc[a] != -2) {
			merge(seg_cc[a], conn);
			return;
		}
		self(a.c(0), conn);
		self(a.c(1), conn);
		update_node(a);
		assert(seg_cc[a] != -1);
		assert(seg_cc[a] != -2);
	});

	for (auto [t, ys] : x_evts) {
		if ((t&1) == 1) {
			// delete these lines
			for (int y : ys) {
				auto p = layout.get_point(y);
				p.for_each([&](auto a) { seg_cnt[a] --; });

				seg_cc[p] = -1;
				p.for_parents_up(update_node);
			}
		}
		int cur_inter = 0;
		{
			auto rng = layout.get_range(ys);
			int conn = t/2;
			rng.for_each([&](seg_tree::point a) {
				cur_inter += seg_cnt[a];
				// now, merge them all
				connect_node(a, conn);
			});
			rng.for_parents_up(update_node);
		}
		num_inter += cur_inter;
		int cur_color = 0;
		{
			auto rng = layout.get_range(0, ys[0]);
			rng.for_each([&](seg_tree::point a) {
				cur_color += seg_cnt[a];
			});
		}
		int nxt_color = cur_color + cur_inter;
		if ((t&1) == 0) {
			rect_color[t/2] = (cur_color & 1) ? -1 : +1;
		}
		vert_black_minus_white += (cur_color & 1) ? -1 : +1;
		vert_black_minus_white += (nxt_color & 1) ? -1 : +1;

		if ((t&1) == 0) {
			// insert these lines
			for (int y : ys) {
				auto p = layout.get_point(y);
				p.for_each([&](auto a) { seg_cnt[a] ++; });

				seg_cc[p] = t/2;
				p.for_parents_up(update_node);
			}
		}
	}

	int num_white_ccs = 0;
	int num_black_ccs = 0;

	std::vector<int> cc_color(N);
	for (auto [t, ys] : x_evts) {
		if ((t&1) == 0) {
			int i = t/2;
			int c = get_par(i);
			if (!cc_color[c]) {
				cc_color[c] = rect_color[i];
				if (rect_color[i] == 1) num_white_ccs++;
				else num_black_ccs++;
			}
		}
	}

	//assert(num_white_ccs + num_black_ccs == 1);
	int64_t num_white = (num_inter * 2 - vert_black_minus_white) / 4 + 1 + num_white_ccs;
	int64_t num_black = (num_inter * 2 + vert_black_minus_white) / 4 + num_black_ccs;

	if (T == 1) {
		cout << num_white + num_black << '\n';
	} else if (T == 2) {
		cout << num_white << ' ' << num_black << '\n';
	} else assert(false);

	return 0;
}
