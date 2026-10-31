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

	// Useful when you want to traverse the tree without calling downdate
	template <typename F, typename PushFunc, typename PopFunc> void euler_tour_l_to_r(F f, PushFunc push, PopFunc pop) const {
		// TODO
		int anc_depth = log_2((a-1) ^ b);
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;

	vector<array<int, 5>> queries(Q);
	vector<int> ans(Q);

	vector<vector<int>> evts(N+1);

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int L, R; cin >> L >> R; L--;
			int C, K; cin >> C >> K;
			queries[q] = {op, L, R, K, C};
			evts[L].push_back(q);
			evts[R].push_back(q);
		} else if (op == 2) {
			int L, R; cin >> L >> R; L--;
			int K; cin >> K;
			queries[q] = {op, L, R, K};
			evts[L].push_back(q);
			evts[R].push_back(q);
		} else if (op == 3) {
			int A; int64_t K; cin >> A >> K; A--;
			queries[q] = {op, A, int(uint32_t(uint64_t(K) >> 32)), int(uint32_t(K))};
			evts[A].push_back(q);
		}
	}

	seg_tree::in_order_layout layout(Q);
	struct seg_node {
		int64_t tot_plus = 0;
		int64_t tot_net = 0;
		int64_t max_net = 0;
	};
	vector<seg_node> seg(2*Q);
	auto update_node = [&](seg_tree::point a) -> void {
		assert(a < Q);
		seg[a].tot_plus = seg[a[0]].tot_plus + seg[a[1]].tot_plus;
		seg[a].tot_net = seg[a[0]].tot_net + seg[a[1]].tot_net;
		seg[a].max_net = max(seg[a[0]].max_net + seg[a[1]].tot_net, seg[a[1]].max_net);
	};

	for (int i = 0; i < N; i++) {
		// evts[i] is sorted, so we don't need to worry about edge cases
		for (int q : evts[i]) {
			const auto& query = queries[q];
			int op = query[0];
			if (op == 1) {
				assert(query[1] == i || query[2] == i);
				int K = query[3];

				seg_tree::point a = layout.get_point(q);
				if (query[1] == i) {
					seg[a] = {K, K, K};
				} else {
					seg[a] = {0,0,0};
				}
				a.for_parents_up(update_node);
			} else if (op == 2) {
				assert(query[1] == i || query[2] == i);
				int K = query[3];

				seg_tree::point a = layout.get_point(q);
				if (query[1] == i) {
					seg[a] = {0, -K, 0};
				} else {
					seg[a] = {0, 0, 0};
				}
				a.for_parents_up(update_node);
			} else if (op == 3) {
				assert(query[1] == i);
				int64_t K = int64_t((uint64_t(uint32_t(query[2])) << 32) | uint64_t(uint32_t(query[3])));

				seg_tree::range r = layout.get_range(0, q);
				int64_t alive_cnt = 0;
				r.for_each_l_to_r([&](seg_tree::point a) -> void {
					alive_cnt = max(alive_cnt + seg[a].tot_net, seg[a].max_net);
				});
				K = alive_cnt - K;
				ans[q] = 0;
				r.for_each_r_to_l([&](seg_tree::point a) -> void {
					if (K < 0) {
						return;
					} else if (K >= seg[a].tot_plus) {
						K -= seg[a].tot_plus;
						return;
					}
					assert(0 <= K && K < seg[a].tot_plus);
					while (a < Q) {
						if (K < seg[a[1]].tot_plus) {
							a = a[1];
						} else {
							K -= seg[a[1]].tot_plus;
							a = a[0];
						}
						assert(0 <= K && K < seg[a].tot_plus);
					}
					assert(0 <= K && K < seg[a].tot_plus);
					assert(a >= Q);
					ans[q] = queries[layout.get_leaf_index(a)][4];
					K -= seg[a].tot_plus;
				});
			} else assert(false);
		}
	}

	for (int q = 0; q < Q; q++) {
		if (queries[q][0] == 3) {
			cout << ans[q] << '\n';
		}
	}

	return 0;
}
