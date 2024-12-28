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
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		const int INF = 1 << 30;
		struct seg_t {
			int len;
			int val; // val assuming no more
			int deficit; // cost for the first extra one, capped at INF
		};
		auto merge = [&](seg_t a, seg_t b) -> seg_t {
			if (a.len == 0) return b;
			if (b.len == 0) return a;
			assert(a.len >= 1 && b.len >= 1);
			assert(b.val < INF);
			// first apply b.val
			if (b.val < a.deficit) {
				if (a.deficit < INF) {
					a.deficit -= b.val;
				}
			} else {
				b.val -= a.deficit;
				a.val++;
				assert(b.val >= 0);
				if (a.len < 30) {
					a.val += (b.val >> a.len);
					b.val &= (1 << a.len) - 1;
					a.deficit = (1 << a.len) - b.val;
					assert(a.deficit > 0);
				} else {
					a.deficit = INF;
				}
			}

			// now b.deficit
			// ((a.deficit - 1) << b.len) + b.deficit;
			assert(a.deficit > 0);
			if (a.deficit < INF) {
				int64_t cnd = (int64_t(a.deficit - 1) << std::min(31, b.len)) + b.deficit;
				if (cnd < INF) a.deficit = int(cnd);
				else a.deficit = INF;
			}
			a.len += b.len;
			return a;
		};
		seg_tree::in_order_layout layout(N);
		std::vector<seg_t> seg_forwards(2*N);
		std::vector<seg_t> seg_backwards(2*N);
		auto unit = [&](int v) -> seg_t {
			return {1, v / 2, 2 - (v % 2)};
		};
		for (int i = 0; i < N; i++) {
			auto a = layout.get_point(i);
			seg_forwards[a] = seg_backwards[a] = unit(A[i]);
		}
		for (seg_tree::point a(N-1); a > 0; a--) {
			seg_forwards[a] = merge(seg_forwards[a.c(0)], seg_forwards[a.c(1)]);
			seg_backwards[a] = merge(seg_backwards[a.c(1)], seg_backwards[a.c(0)]);
		}

		const seg_t ID{0, 0, 1};


		for (int i = 0; i < N; i++) {
			int max_try = std::min(30, N-1);
			auto query_forwards = [&](int sz) -> seg_t {
				seg_t res = ID;
				if (i + 1 + sz > N) {
					layout.get_range(i+1, N).for_each_l_to_r([&](auto a) -> void {
						res = merge(res, seg_forwards[a]);
					});
					layout.get_range(0, i+1+sz-N).for_each_l_to_r([&](auto a) -> void {
						res = merge(res, seg_forwards[a]);
					});
				} else {
					layout.get_range(i+1, i+1+sz).for_each_l_to_r([&](auto a) -> void {
						res = merge(res, seg_forwards[a]);
					});
				}
				return res;
			};
			std::vector<seg_t> forwards_vals(max_try+1);
			forwards_vals[max_try] = query_forwards(N - 1 - max_try);
			for (int d = max_try; d > 0; d--) {
				int j = (i - d + N) % N;
				forwards_vals[d-1] = merge(forwards_vals[d], unit(A[j]));
			}
			seg_t backwards_val = ID;
			int64_t best = 0;
			for (int d = 0; d <= max_try; d++) {
				if (d) {
					int j = (i - d + N) % N;
					backwards_val = merge(backwards_val, unit(A[j]));
				}
				best = std::max(best, int64_t(backwards_val.val) + int64_t(forwards_vals[d].val) + int64_t(A[i]));
			}
			int tgt = backwards_val.val + 1;
			bool done = false;
			seg_t cur = ID;
			seg_t other = ID;
			auto check_range = [&](seg_tree::point a) -> void {
				if (done) {
					other = merge(seg_forwards[a], other);
					return;
				}
				seg_t cnd = merge(cur, seg_backwards[a]);
				if (cnd.val < tgt) {
					cur = cnd;
					return;
				}
				assert(cnd.val >= tgt);
				while (a < layout.N) {
					assert(cur.val < tgt);
					cnd = merge(cur, seg_backwards[a.c(1)]);
					if (cnd.val >= tgt) {
						other = merge(other, seg_forwards[a.c(0)]);
						a = a.c(1);
					} else {
						cur = cnd;
						a = a.c(0);
					}
				}
				assert(cur.val < tgt);
				cur = merge(cur, seg_backwards[a]);
				assert(cur.val >= tgt);
				done = true;
			};
			layout.get_range(0, i).for_each_r_to_l(check_range);
			layout.get_range(i+1, N).for_each_r_to_l(check_range);
			best = std::max(best, int64_t(other.val) + int64_t(cur.val) + int64_t(A[i]));
		}
	}

	return 0;
}
