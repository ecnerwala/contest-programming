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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;
	std::vector<int> ops(M);
	for (auto& a : ops) cin >> a;

	seg_tree::in_order_layout layout(N+1 + 2*M);

	struct seg_node {
		std::array<int, 2> ch{0, 0};
		int val = 0;
	};

	std::vector<seg_node> nodes; nodes.reserve(2 * layout.N + 40 * M); nodes.resize(2 * layout.N);
	for (int i = 0; i < layout.N; i++) {
		nodes[layout.get_point(i)].val = 1;
	}
	for (seg_tree::point a(layout.N-1); a > 0; a--) {
		nodes[a].ch = {a.c(0), a.c(1)};
		nodes[a].val = nodes[a.c(0)].val + nodes[a.c(1)].val;
	}
	std::vector<int> roots; roots.reserve(M+1);
	int root = 1;
	roots.push_back(root);
	for (int t = 0; t < M; t++) {
		int op = ops[t] + (M+1 - t) - 1; // we want to delete the op'th gap and the op+1'th gap
		for (int z = 0; z < 2; z++) {
			cerr << "remove " << op << " from " << nodes[root].val << '\n';
			root = [&](this auto&& self, int cur, seg_tree::point a, int kth) -> int {
				assert(0 <= kth && kth < nodes[cur].val);
				if (a >= layout.N) {
					assert(nodes[cur].val == 1);
					int nxt = int(nodes.size());
					nodes.push_back(nodes[cur]);
					nodes[nxt].val = 0;
					return nxt;
				}
				assert(a < layout.N);
				assert(nodes[cur].ch[0] != 0);
				assert(nodes[cur].ch[1] != 0);
				int d = kth >= nodes[nodes[cur].ch[0]].val;
				kth = d ? kth - nodes[nodes[cur].ch[0]].val : kth;
				int nch = self(nodes[cur].ch[d], a.c(d), kth);
				int nxt = int(nodes.size());
				nodes.push_back(nodes[cur]);
				nodes[nxt].val--;
				nodes[nxt].ch[d] = nch;
				return nxt;
			}(root, seg_tree::point(1), op);
		}
		roots.push_back(root);
	}

	auto get_strand = [&](int t, int p) -> int {
		assert(0 <= t && t <= M);
		assert(-1 <= p && p <= N);
		p += M+1-t;
		int cur = roots[t];
		seg_tree::point a(1);
		while (a < layout.N) {
			int d = p >= nodes[nodes[cur].ch[0]].val;
			p = d ? p - nodes[nodes[cur].ch[0]].val : p;
			cur = nodes[cur].ch[d];
			a = a.c(d);
		}
		return layout.get_leaf_index(a);
	};
	auto get_loc = [&](int t, int p) -> int {
		int ans = 0;
		int cur = roots[t];
		seg_tree::point a(1);
		while (a < layout.N) {
			int d = p >= layout.get_node_split(a);
			ans += d ? nodes[nodes[cur].ch[0]].val : 0;
			cur = nodes[cur].ch[d];
			a = a.c(d);
		}
		assert(p == layout.get_leaf_index(a));
		ans -= M+1-t;
		return ans;
	};

	int prv_ans = 0;
	for (int q = 0; q < Q; q++) {
		int op, t0, t1, p; cin >> op >> t0 >> t1 >> p;
		op = (op + prv_ans) % 2;
		t0 = (t0 + prv_ans) % M;
		t1 = (t1 + prv_ans) % M;
		if (t0 > t1) std::swap(t0, t1);
		assert(t0 <= t1);
		p = (p + prv_ans) % N;
		t1++;
		assert(0 <= t0 && t0 < t1 && t1 <= M);

		//cerr << "query " << op << ' ' << t0 << ' ' << t1 << ' ' << p << '\n';

		int nxt_ans;
		if (op == 0) {
			// start at time t0
			int strand = get_strand(t0, p);
			// we want to get strand's
			//cerr << "got strand " << strand << '\n';
			nxt_ans = get_loc(t1, strand);
		} else if (op == 1) {
			int a = get_strand(t1, p-1);
			int b = get_strand(t1, p);
			//cerr << "got locs " << a << ' ' << b << '\n';
			b = get_loc(t0, b);
			a = get_loc(t0, a);
			//cerr << "got orig strands" << a << ' ' << b << '\n';
			// a+1 .. b inclusive
			// we want [a+1, b] intersect [0, N-1]
			b = std::min(b, N-1);
			a = std::max(a, -1);
			nxt_ans = std::max(b - a, 0);
			//nxt_ans = get_loc(t0, b) - get_loc(t0, a);
		} else assert(false);
		cout << nxt_ans << '\n';
		//cerr << "ans " << nxt_ans << '\n';
		prv_ans = nxt_ans;
	}
	return 0;
}
