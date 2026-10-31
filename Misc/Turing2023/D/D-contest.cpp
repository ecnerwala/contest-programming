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

	struct op_t {
		int type; // 1 is setmax, 2 is setmin
		std::array<int, 2> bounds;
		int c;
	};
	std::vector<op_t> ops(Q);
	for (auto& op : ops) {
		cin >> op.type >> op.bounds[0] >> op.bounds[1] >> op.c;
		op.bounds[0]--;
		assert(op.type == 1 || op.type == 2);
		assert(0 <= op.bounds[0] && op.bounds[0] < op.bounds[1] && op.bounds[1] <= N);
	}

	if (N * Q <= int(4e6)) {
		std::vector<int64_t> ans(Q);
		std::vector<std::pair<int, int>> los; los.reserve(Q+1);
		std::vector<std::pair<int, int>> his; his.reserve(Q+1);

		for (int i = 0; i < N; i++) {
			los.clear();
			los.push_back({0, 0});
			his.clear();
			his.push_back({0, 0});

			int lo_front = 0;
			int hi_front = 0;

			int64_t cur_val = 0;

			for (int q = 0; q < Q; q++) {
				const auto& op = ops[q];
				if (op.bounds[0] <= i && i < op.bounds[1]) {
					if (op.type == 1) {
						while (true) {
							if (lo_front == int(los.size())) {
								while (hi_front+1 < int(his.size()) && his[hi_front+1].first <= op.c) {
									hi_front++;
								}
								his[hi_front].first = op.c;
								cur_val = int64_t(his[hi_front].second) * int64_t(op.c);
								break;
							} else if (op.c >= los.back().first) {
								los.pop_back();
								continue;
							} else {
								break;
							}
						}
						los.push_back({op.c, q+1});
					} else if (op.type == 2) {
						while (true) {
							if (hi_front == int(his.size())) {
								while (lo_front+1 < int(los.size()) && los[lo_front+1].first >= op.c) {
									lo_front++;
								}
								los[lo_front].first = op.c;
								cur_val = int64_t(los[lo_front].second) * int64_t(op.c);
								break;
							} else if (op.c <= his.back().first) {
								his.pop_back();
								continue;
							} else {
								break;
							}
						}
						his.push_back({op.c, q+1});
					} else assert(false);
				}
				ans[q] += cur_val;
			}
		}
		for (int q = 0; q < Q; q++) {
			cout << ans[q] << " \n"[q+1==Q];
		}
		exit(0);

	}

	int num_type_2 = 0;
	for (int q = 0; q < Q; q++) {
		num_type_2 += ops[q].type == 2;
	}
	if (num_type_2 == 0) {
		// don't need to do anything, never stable
		for (int q = 0; q < Q; q++) {
			cout << 0 << " \n"[q+1==Q];
		}
		exit(0);
	} else if (num_type_2 == 1) {
		int type_2_idx = 0;
		while (ops[type_2_idx].type != 2) type_2_idx++;
		int v_hi = ops[type_2_idx].c;

		seg_tree::in_order_layout layout(N);

		std::vector<int> seg_early(2*N, 0);
		for (int q = 0; q < type_2_idx; q++) {
			assert(ops[q].type == 1);
			if (ops[q].c < v_hi) continue;
			auto rng = layout.get_range(ops[q].bounds);
			rng.for_each([&](seg_tree::point a) {
				seg_early[a] = q+1;
			});
		}
		for (seg_tree::point a(2); a < 2*N; a++) {
			seg_early[a] = std::max(seg_early[a], seg_early[a/2]);
		}

		const int64_t INF = 2e18;
		struct seg_t {
			int64_t v_min;
			int64_t cnt_min;
			int64_t v_min_2;
			int64_t sum;
		};
		std::vector<seg_t> seg(2*N);
		auto apply_easy = [&](seg_tree::point a, int64_t v) -> void {
			assert(v < seg[a].v_min_2);
			if (v <= seg[a].v_min) return;
			int64_t diff = v - seg[a].v_min;
			seg[a].v_min = v;
			seg[a].sum += diff * seg[a].cnt_min;
		};
		auto downdate_node = [&](seg_tree::point a) -> void {
			if (a >= N) return;
			apply_easy(a.c(0), seg[a].v_min);
			apply_easy(a.c(1), seg[a].v_min);
		};
		auto update_node = [&](seg_tree::point a) -> void {
			if (a >= N) {
				return;
			} else {
				const auto& c0 = seg[a.c(0)];
				const auto& c1 = seg[a.c(1)];
				seg[a].v_min = std::min(c0.v_min, c1.v_min);
				seg[a].cnt_min = (c0.v_min == seg[a].v_min ? c0.cnt_min : 0) + (c1.v_min == seg[a].v_min ? c1.cnt_min : 0);
				seg[a].v_min_2 = (c0.v_min == seg[a].v_min ? c0.v_min_2 : c0.v_min) + (c1.v_min == seg[a].v_min ? c1.v_min_2 : c1.v_min);
				seg[a].sum = c0.sum + c1.sum;
			}
		};
		auto apply_hard = std::y_combinator([&](auto self, seg_tree::point a, int64_t v) -> void {
			if (v < seg[a].v_min_2) {
				apply_easy(a, v);
				return;
			}
			assert(a < N);
			downdate_node(a);
			self(a.c(0), v);
			self(a.c(1), v);
			update_node(a);
		});
		for (int i = 0; i < N; i++) {
			auto a = layout.get_point(i);
			seg[a].v_min = (ops[type_2_idx].bounds[0] <= i && i < ops[type_2_idx].bounds[1]) ? int64_t(v_hi) * int64_t(seg_early[a]) : 0;
			seg[a].v_min_2 = INF;
			seg[a].cnt_min = 1;
			seg[a].sum = seg[a].v_min;
		}
		for (seg_tree::point a(N-1); a > 0; a--) update_node(a);

		auto get_ans = [&]() -> int64_t {
			return seg[1].sum;
		};

		std::vector<int64_t> ans(Q);
		ans[type_2_idx] = get_ans();
		for (int q = type_2_idx+1; q < Q; q++) {
			auto& op = ops[q];
			assert(op.type == 1);
			op.bounds[0] = std::max(op.bounds[0], ops[type_2_idx].bounds[0]);
			op.bounds[1] = std::min(op.bounds[1], ops[type_2_idx].bounds[1]);
			if (op.bounds[0] < op.bounds[1]) {
				int64_t new_val = int64_t(op.c) * int64_t(type_2_idx+1);
				auto rng = layout.get_range(op.bounds);
				rng.for_parents_down(downdate_node);
				rng.for_each([&](seg_tree::point a) {
					apply_hard(a, new_val);
				});
				rng.for_parents_up(update_node);
			}
			ans[q] = get_ans();
		}

		for (int q = 0; q < Q; q++) {
			cout << ans[q] << " \n"[q+1==Q];
		}
		exit(0);
	}

	return 0;
}
