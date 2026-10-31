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

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::pair<bool, std::array<int, 2>>> V(M);
		for (auto& [t, a] : V) { cin >> t >> a[0] >> a[1]; a[0]--; }

		seg_tree::in_order_layout layout(N);
		struct seg_zero_t {
			int lazy = 0;
			bool full = false;
		};
		std::vector<seg_zero_t> seg_zero(2*layout.N);
		auto seg_zero_update_node = [&](seg_tree::point p) {
			assert(seg_zero[p].lazy >= 0);
			seg_zero[p].full = seg_zero[p].lazy || (p < layout.N && seg_zero[p.c(0)].full && seg_zero[p.c(1)].full);
		};

		std::vector<int> seg_one(2 * layout.N, N+1);
		std::vector<std::multiset<int>> live(N);
		for (int i = 0; i < N; i++) live[i].insert(N+1);
		auto seg_one_update_node = [&](seg_tree::point p) {
			if (p < layout.N) seg_one[p] = std::min(seg_one[p.c(0)], seg_one[p.c(1)]);
			else {
				int i = layout.get_leaf_index(p);
				seg_one[p] = *live[i].begin();
			}
		};

		std::vector<int> min_l(M+1);
		for (int l = 0, r = 0; r < M; r++) {
			auto [t, a] = V[r];
			while (true) {
				if ([&]() -> bool {
					if (t == 0) {
						// check if this interval now contains something
						// get next zero from each of these
						int prv_allowed = [&](this auto&& self, seg_tree::point cur) -> std::optional<int> {
							if (seg_zero[cur].full) return std::nullopt;
							if (cur >= layout.N) {
								return layout.get_leaf_index(cur);
							}
							int m = layout.get_node_split(cur);
							if (a[0] > m) {
								auto ch = self(cur.c(1));
								if (ch) return ch;
							}
							return self(cur.c(0));
						}(seg_tree::point(1)).value_or(-1);
						int nxt_allowed = [&](this auto&& self, seg_tree::point cur) -> std::optional<int> {
							if (seg_zero[cur].full) return std::nullopt;
							if (cur >= layout.N) {
								return layout.get_leaf_index(cur);
							}
							int m = layout.get_node_split(cur);
							if (a[1] < m) {
								auto ch = self(cur.c(0));
								if (ch) return ch;
							}
							return self(cur.c(1));
						}(seg_tree::point(1)).value_or(N);

						// check if there's anything between prv_allowed and nxt_allowed
						bool bad = false;
						layout.get_range(prv_allowed + 1, nxt_allowed).for_each([&](seg_tree::point p) -> void {
							if (seg_one[p] <= nxt_allowed) {
								bad = true;
							}
						});
						return !bad;
					} else if (t == 1) {
						// check if there's an empty guy
						return [&](this auto&& self, seg_tree::point cur) -> bool {
							if (seg_zero[cur].full) return false;
							auto [x, y] = layout.get_node_bounds(cur);
							if (a[1] <= x || a[0] >= y) return false;
							if (a[0] <= x && y <= a[1]) return true;
							return self(cur.c(0)) || self(cur.c(1));
						}(seg_tree::point(1));
					} else assert(false);
				}()) {
					// add r
					min_l[r] = l;
					if (t == 0) {
						auto rng = layout.get_range(a);
						rng.for_each([&](auto p) -> void {
							seg_zero[p].lazy++;
							seg_zero_update_node(p);
						});
						rng.for_parents_up(seg_zero_update_node);
					} else if (t == 1) {
						auto [x, y] = a;
						live[x].insert(y);
						layout.get_point(x).for_each_up(seg_one_update_node);
					} else assert(false);
					break;
				} else {
					assert(l < r);
					// delete l
					if (V[l].first == 0) {
						auto rng = layout.get_range(V[l].second);
						rng.for_each([&](auto p) -> void {
							seg_zero[p].lazy--;
							seg_zero_update_node(p);
						});
						rng.for_parents_up(seg_zero_update_node);
					} else if (V[l].first == 1) {
						auto [x, y] = V[l].second;
						live[x].erase(live[x].find(y));
						layout.get_point(x).for_each_up(seg_one_update_node);
					} else assert(false);
					l++;
				}
			}
		}


		int Q; cin >> Q;
		while (Q--) {
			int l, r; cin >> l >> r; l--, r--;
			if (l >= min_l[r]) {
				cout << "YES" << '\n';
			} else {
				cout << "NO" << '\n';
			}
		}
	}

	return 0;
}
