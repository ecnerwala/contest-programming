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

	template <typename F> void for_each(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++), false);
			if (y & 1) f(point(--y), true);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a >> __builtin_ctz(a);
		int y = b >> __builtin_ctz(b);

		// TODO: dedup
		point(x).for_parents_down(f);
		point(y).for_parents_down(f);
	}

	template <typename F> void for_parents_up(F f) const {
		int x = a >> __builtin_ctz(a);
		int y = b >> __builtin_ctz(b);

		// TODO: dedup
		point(x).for_parents_up(f);
		point(y).for_parents_up(f);
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

	int get_node_size(point pt) const {
		auto [x, y] = get_node_bounds(pt);
		int r = y - x;
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q; cin >> N >> Q;
	const int INF = 1 << 30;

	auto seg_layout = seg_tree::circular_layout(N);

	struct seg_node {
		int min_val;
		int min_cnt;
		int min_val_2;
		std::array<int, 30> bit_cnts;
	};
	vector<seg_node> seg(2*N);

	auto update_node = [&](int a) {
		assert(a < N);
		int l = 2*a, r = 2*a+1;
		seg[a].min_val = min(seg[l].min_val, seg[r].min_val);
		seg[a].min_cnt =
			(seg[l].min_val == seg[a].min_val ? seg[l].min_cnt : 0)
			+ (seg[r].min_val == seg[a].min_val ? seg[r].min_cnt : 0);

		seg[a].min_val_2 = min(
			(seg[l].min_val == seg[a].min_val ? seg[l].min_val_2 : seg[l].min_val),
			(seg[r].min_val == seg[a].min_val ? seg[r].min_val_2 : seg[r].min_val)
		);
		for (int b = 0; b < 30; b++) {
			seg[a].bit_cnts[b] = seg[l].bit_cnts[b] + seg[r].bit_cnts[b];
		}
	};
	auto apply_update = std::y_combinator([&](auto self, int a, int x) -> void {
		if (x <= seg[a].min_val) {
			// doesn't do anything
			return;
		} else if (x < seg[a].min_val_2) {
			// only affects seg[a].min_val
			for (int b = 0; b < 30; b++) {
				seg[a].bit_cnts[b] -= ((seg[a].min_val >> b) & 1) * seg[a].min_cnt;
				seg[a].bit_cnts[b] += ((x >> b) & 1) * seg[a].min_cnt;
			}

			seg[a].min_val = x;
		} else {
			assert(x >= seg[a].min_val); // no need to downdate
			assert(a < N);
			self(2*a, x);
			self(2*a+1, x);
			update_node(a);
		}
	});

	auto downdate_node = [&](int a) {
		assert(a < N);
		apply_update(2*a, seg[a].min_val);
		apply_update(2*a+1, seg[a].min_val);
		update_node(a);
	};

	for (int i = 0; i < N; i++) {
		int a = N+i;
		cin >> seg[a].min_val;
		seg[a].min_cnt = 1;
		seg[a].min_val_2 = INF;
		for (int b = 0; b < 30; b++) seg[a].bit_cnts[b] = ((seg[a].min_val >> b) & 1);
	}
	for (int a = N-1; a; a--) update_node(a);

	while (Q--) {
		int op, l, r, x; cin >> op >> l >> r >> x;
		l--;
		auto cur_range = seg_layout.get_range(l, r);
		cur_range.for_parents_down(downdate_node);

		if (op == 1) {
			cur_range.for_each([&](auto a, bool) {
				apply_update(a, x);
			});
			cur_range.for_parents_up(update_node);
		} else if (op == 2) {
			std::array<int, 30> bit_cnts;
			for (int b = 0; b < 30; b++) {
				bit_cnts[b] = ((x >> b) & 1);
			}
			cur_range.for_each([&](auto a, bool) {
				for (int b = 0; b < 30; b++) {
					bit_cnts[b] += seg[a].bit_cnts[b];
				}
			});

			int ans = 0;
			for (int b = 29; b >= 0; b--) {
				if (bit_cnts[b] & 1) {
					ans = bit_cnts[b];
					break;
				}
			}
			cout << ans << '\n';
		} else assert(false);
	}

	return 0;
}
