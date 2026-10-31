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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		struct par_t {
			int par;
			int depth;
			int jump_par;
			int jump_dist;
			int jump_min;
			int jump_max;
		};
		std::vector<par_t> pars(N);
		std::y_combinator([&](auto self, int cur, int prv, int d) -> void {
			pars[cur].par = prv;
			pars[cur].depth = d;
			if (prv == -1) {
				pars[cur].jump_par = -1;
				pars[cur].jump_dist = -1;
			} else if (pars[prv].jump_par != -1 && pars[prv].jump_dist == pars[pars[prv].jump_par].jump_dist) {
				int gp = pars[prv].jump_par;
				pars[cur].jump_par = pars[gp].jump_par;
				pars[cur].jump_dist = 1 + 2 * pars[prv].jump_dist;
				pars[cur].jump_min = std::min({cur, pars[prv].jump_min, pars[gp].jump_min});
				pars[cur].jump_max = std::max({cur, pars[prv].jump_max, pars[gp].jump_max});
			} else {
				pars[cur].jump_par = prv;
				pars[cur].jump_dist = 1;
				pars[cur].jump_min = std::min({cur, prv});
				pars[cur].jump_max = std::max({cur, prv});
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur, d+1);
			}
		})(0, -1, 0);

		auto get_path_bounds = [&](int a, int b) -> std::array<int, 2> {
			int mi = std::min(a, b);
			int ma = std::max(a, b);
			if (pars[a].depth > pars[b].depth) {
				std::swap(a, b);
			}
			while (pars[b].depth > pars[a].depth) {
				if (pars[b].jump_dist <= pars[b].depth - pars[a].depth) {
					mi = std::min(mi, pars[b].jump_min);
					ma = std::max(ma, pars[b].jump_max);
					b = pars[b].jump_par;
				} else {
					b = pars[b].par;
					mi = std::min(mi, b);
					ma = std::max(ma, b);
				}
			}
			if (a != b) {
				while (pars[a].par != pars[b].par) {
					assert(pars[a].depth == pars[b].depth);
					assert(pars[a].jump_dist == pars[b].jump_dist);
					if (pars[a].jump_par != pars[b].jump_par) {
						mi = std::min(mi, pars[a].jump_min);
						ma = std::max(ma, pars[a].jump_max);
						a = pars[a].jump_par;
						mi = std::min(mi, pars[b].jump_min);
						ma = std::max(ma, pars[b].jump_max);
						b = pars[b].jump_par;
					} else {
						a = pars[a].par;
						mi = std::min(mi, a);
						ma = std::max(ma, a);
						b = pars[b].par;
						mi = std::min(mi, b);
						ma = std::max(ma, b);
					}
				}
				assert(pars[a].par == pars[b].par);
				int p = pars[a].par;
				assert(p != -1);
				mi = std::min(mi, p);
				ma = std::max(ma, p);
			}
			return {mi, ma};
		};

		std::vector<std::vector<std::pair<std::array<int, 2>, int>>> ops(N+1);
		// inclusive ranges
		auto add_rect = [&](int x_lo, int x_hi, int y_lo, int y_hi) -> void {
			x_hi++;
			y_hi++;
			assert(x_lo <= x_hi);
			assert(y_lo <= y_hi);
			assert(0 <= x_lo && x_lo <= x_hi && x_hi <= N);
			assert(0 <= y_lo && y_lo <= y_hi && y_hi <= N);
			ops[x_lo].push_back({{y_lo, y_hi}, +1});
			ops[x_hi].push_back({{y_lo, y_hi}, -1});
		};
		for (int i = 0; i+1 < N; i++) {
			add_rect(i+1, N-1, 0, i);
		}
		for (int i = 0; i+1 < N; i++) {
			auto [lo, hi] = get_path_bounds(i, i+1);
			assert(lo <= i && i+1 <= hi);
			// exclude (lo+1..i) x (i+1..N)
			add_rect(lo+1, i, i+1, N-1);
			add_rect(0, i, i+1, hi-1);
		}

		seg_tree::in_order_layout layout(N);
		struct seg_t {
			int mark = 0;
			int sum = 0;
		};
		std::vector<seg_t> seg(2*N);
		auto update_node = [&](seg_tree::point a) -> void {
			assert(seg[a].mark >= 0);
			if (seg[a].mark) {
				seg[a].sum = 0;
			} else if (a >= N) {
				seg[a].sum = 1;
			} else {
				seg[a].sum = seg[a.c(0)].sum + seg[a.c(1)].sum;
			}
		};
		for (seg_tree::point a(2*N-1); a > 0; a--) update_node(a);

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			for (auto [ys, delta_] : ops[i]) {
				int delta = delta_;
				auto rng = layout.get_range(ys);
				rng.for_each([&](auto a) { seg[a].mark += delta; update_node(a); });
				rng.for_parents_up(update_node);
			}
			ans += seg[1].sum;
		}
		cout << ans << '\n';
	}

	return 0;
}
