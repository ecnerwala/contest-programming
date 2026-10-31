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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false);

	int T; cin >> T;
	while (T--) {
		//cerr << "start test" << '\n';
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }
		//for (int i = 0; i < N; i++) { cerr << P[i] << " \n"[i+1==N]; }
		std::vector<int> inv_P(N);
		for (int i = 0; i < N; i++) {
			inv_P[P[i]] = i;
		}

		std::vector<std::vector<std::pair<int, int>>> inserts(N);
		std::vector<std::vector<std::pair<int, int>>> deletes(N);

		auto add_rectangle = [&](int xlo, int xhi, int ylo, int yhi) {
			if (xlo > xhi || ylo > yhi) return;
			inserts[xlo].push_back({ylo, yhi});
			deletes[xhi].push_back({ylo, yhi});
		};
		auto add_point = [&](int x, int y) {
			if (x < 0 || x >= N || y < 0 || y >= N) return;
			assert(x <= y);
			add_rectangle(x, x, y, y);
		};

		int mi0 = inv_P[0];
		int mi1 = N;
		int ma0 = inv_P[0];
		int ma1 = -1;
		int64_t sum_idx = inv_P[0];
		for (int v = 1; v < N; v++) {
			int i = inv_P[v];
			sum_idx += i;
			if (i < mi0) {
				mi1 = mi0;
				mi0 = i;
			} else if (i < mi1) {
				mi1 = i;
			}
			if (i > ma0) {
				ma1 = ma0;
				ma0 = i;
			} else if (i > ma1) {
				ma1 = i;
			}

			int l = v+1;
			// now, try moving each of them
			assert(ma0 - mi0 + 1 >= l);
			if (ma0 - mi0 + 1 == l) {
				// we can also cycle it potentially
				add_rectangle(mi0, ma0, mi0, ma0);

				add_rectangle(0, mi0-1, 0, mi0-1);
				add_rectangle(ma0+1, N-1, ma0+1, N-1);
				add_rectangle(0, mi0 - 1, ma0 + 1, N-1);

				add_point(mi0, ma0 + 1);
				add_point(mi0 - 1, ma0);
			} else {
				// try moving mi0
				if (ma0 - mi1 + 1 == l-1) {
					add_point(mi0, mi1 - 1);
					add_point(mi0, ma0 + 1);
				} else if (ma0 - mi1 + 1 == l) {
					int missing = int(int64_t(ma0 - mi1 + 1) * int64_t(mi1 + ma0) / 2 - (sum_idx - mi0));
					assert(mi1 <= missing && missing <= ma0 && P[missing] > v);
					add_point(mi0, missing);
				} else {
					// nothing good
				}

				if (ma1 - mi0 + 1 == l-1) {
					add_point(mi0 - 1, ma0);
					add_point(ma1 + 1, ma0);
				} else if (ma1 - mi0 + 1 == l) {
					int missing = int(int64_t(ma1 - mi0 + 1) * int64_t(mi0 + ma1) / 2 - (sum_idx - ma0));
					assert(mi0 <= missing && missing <= ma1 && P[missing] > v);
					add_point(missing, ma0);
				} else {
					// nothing good
				}
			}
		}

		seg_tree::in_order_layout layout(N);
		struct seg_node {
			std::pair<int, int> v;
			int lazy;
		};
		std::vector<seg_node> seg(2*N);
		auto update_node = [&](seg_tree::point a) -> void {
			seg[a].v = std::max(seg[a.c(0)].v, seg[a.c(1)].v);
			seg[a].v.first += seg[a].lazy;
		};
		for (int i = 0; i < N; i++) { seg[layout.get_point(i)] = {{0, i}, 0}; }
		for (seg_tree::point a(N-1); a > 0; a--) update_node(a);

		int best_val = 0;
		int best_i = 0, best_j = 0;

		for (int i = 0; i < N; i++) {
			for (auto [ylo, yhi] : inserts[i]) {
				auto rng = layout.get_range(ylo, yhi+1);
				rng.for_each([&](auto a) {
					seg[a].v.first ++;
					seg[a].lazy ++;
				});
				rng.for_parents_up(update_node);
			}
			if (seg[1].v.first > best_val) {
				best_val = seg[1].v.first;
				best_i = i;
				best_j = seg[1].v.second;
			}
			for (auto [ylo, yhi] : deletes[i]) {
				auto rng = layout.get_range(ylo, yhi+1);
				rng.for_each([&](auto a) {
					seg[a].v.first --;
					seg[a].lazy --;
				});
				rng.for_parents_up(update_node);
			}
		}
		cout << best_i + 1 << ' ' << best_j + 1 << '\n';
	}

	return 0;
}
