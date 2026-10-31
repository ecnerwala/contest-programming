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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int64_t INF = 1e18;

	int N, Q; cin >> N >> Q;
	seg_tree::in_order_layout layout(N);
	struct seg_node {
		int sz;
		array<int64_t, 6> val;
		array<int64_t, 6> lazy;
	};
	vector<seg_node> seg(2*N);
	auto update_node = [&](seg_tree::point a) -> void {
		assert(a < N);
		seg[a].sz = seg[a.c(0)].sz;
		seg[a].val = seg[a.c(0)].val;
		for (int i = 0; i < 6; i++) {
			setmin(seg[a].val[(i+seg[a].sz)%6], seg[a.c(1)].val[i]);
		}
		seg[a].sz += seg[a.c(1)].sz;
	};

	auto downdate_node = [&](seg_tree::point a) -> void {
		assert(a < N);
		for (int z = 0; z < 2; z++) {
			int offset = z ? 6 - (seg[a.c(0)].sz%6) : 0;
			for (int i = 0; i < 6; i++) {
				int64_t v = seg[a].lazy[i];
				seg[a.c(z)].val[(i+offset)%6] += v;
				seg[a.c(z)].lazy[(i+offset)%6] += v;
			}
		}
		seg[a].lazy.fill(0);
	};

	for (int i = N; i < 2 * N; i++) {
		seg[i].sz = 1;
		seg[i].val.fill(INF);
		seg[i].lazy.fill(0);
		seg[i].val[0] = 0;
	}
	for (seg_tree::point a(N-1); a > 0; a--) {
		update_node(a);
	}

	while (Q--) {
		string op; cin >> op;
		if (op == "change") {
			int l, r, k, x; cin >> l >> r >> k >> x; l--;
			std::y_combinator([&](auto self, seg_tree::point a, int lo) -> void {
				int hi = lo + seg[a].sz;
				if (r <= lo || hi <= l) return;
				if (l <= lo && hi <= r) {
					for (int i = 0; i < 6; i++) {
						if ((i + lo - l) % k == 0) {
							seg[a].val[i] += x;
							seg[a].lazy[i] += x;
						}
					}
					return;
				}
				downdate_node(a);
				self(a[0], lo);
				self(a[1], lo + seg[a[0]].sz);
				update_node(a);
			})(seg_tree::point(1), 0);
		} else if (op == "getmin") {
			int l, r, k; cin >> l >> r >> k; l--;
			int64_t res = INF;
			std::y_combinator([&](auto self, seg_tree::point a, int lo) -> void {
				int hi = lo + seg[a].sz;
				if (r <= lo || hi <= l) return;
				if (l <= lo && hi <= r) {
					for (int i = 0; i < 6; i++) {
						if ((i + lo - l) % k == 0) {
							setmin(res, seg[a].val[i]);
						}
					}
					return;
				}
				downdate_node(a);
				self(a[0], lo);
				self(a[1], lo + seg[a[0]].sz);
				update_node(a);
			})(seg_tree::point(1), 0);
			cout << res << '\n';
		} else if (op == "kill") {
			int l; cin >> l; l--;
			seg_tree::point a(1);
			int lo = 0, hi = seg[a].sz;
			assert(lo <= l && l < hi);
			while (a < N) {
				assert(hi - lo == seg[a].sz);
				downdate_node(a);
				int md = lo + seg[a.c(0)].sz;
				if (l < md) {
					hi = md;
					a = a.c(0);
				} else {
					lo = md;
					a = a.c(1);
				}
			}
			assert(lo == l && hi == l + 1);
			seg[a].sz = 0;
			seg[a].val.fill(INF);
			seg[a].lazy.fill(0);
			a.for_parents_up(update_node);
		} else if (op == "revive") {
			int d; cin >> d; d--;
			auto p = layout.get_point(d);
			p.for_parents_down(downdate_node);

			seg[p].sz = 1;
			seg[p].val.fill(INF);
			seg[p].lazy.fill(0);
			seg[p].val[0] = 0;

			p.for_parents_up(update_node);
		} else assert(false);
	}

	return 0;
}
