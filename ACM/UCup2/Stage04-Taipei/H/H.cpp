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
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q, B; cin >> N >> Q >> B;
	seg_tree::in_order_layout layout(N+1);
	std::array<std::vector<uint64_t>, 2> seg;
	for (int z = 0; z < 2; z++) {
		seg[z].resize(2*layout.N);
		for (int i = 0; i < N; i++) {
			cin >> seg[z][layout.get_point(i)];
		}
	}

	std::vector<std::tuple<int, int, int, uint64_t>> queries(Q);
	for (auto& [type, a, b, c] : queries) {
		cin >> type;
		if (type == 1) {
			cin >> a >> b >> c;
			a--, b--;
		} else if (type == 2) {
			cin >> a;
		} else assert(false);
	}

	constexpr bool TESTING = false;
	auto do_add = [&](uint64_t a, uint64_t b) -> uint64_t {
		if (TESTING) {
			cerr << 'A' << ' ' << a << ' ' << b << '\n' << flush;
			return a+b;
		}
		cout << 'A' << ' ' << a << ' ' << b << '\n' << flush;
		uint64_t c; cin >> c;
		return c;
	};
	auto do_min = [&](uint64_t a, uint64_t b) -> uint64_t {
		if (TESTING) {
			cerr << 'C' << ' ' << a << ' ' << b << '\n' << flush;
			return std::min(a, b);
		}
		if (a == b) return a;
		cout << 'C' << ' ' << a << ' ' << b << '\n' << flush;
		uint64_t c; cin >> c;
		return c;
	};

	auto update_node = [&](int z, seg_tree::point a) -> void {
		if (layout.get_node_bounds(a)[1] == N+1) return;
		seg[z][a] = do_add(seg[z][a.c(0)], seg[z][a.c(1)]);
	};

	for (int z = 0; z < 2; z++) {
		for (seg_tree::point a(layout.N - 1); a > 0; a--) {
			update_node(z, a);
		}
	}

	auto solve = [&](int k) -> uint64_t {
		assert(1 <= k && k <= 2*N);
		if (TESTING) {
			cerr << "currrent prefs" << '\n';
			for (int z = 0; z < 2; z++) {
				uint64_t v = 0;
				cerr << ' ' << v << ' ';
				for (int i = 0; i < N; i++) {
					v += seg[z][layout.get_point(i)];
					cerr << v << ' ';
				}
				cerr << '\n';
			}
		}
		// 0-indexed
		// in each seg tree node, we use strictly between 0 and all of the elements
		seg_tree::point a_0(1);
		seg_tree::point a_1(1);

		// nullopt mean 0
		std::optional<uint64_t> v0 = nullopt;
		std::optional<uint64_t> v1 = nullopt;

		// nullopt means unset
		std::optional<uint64_t> cnd0 = nullopt;
		std::optional<uint64_t> cnd1 = nullopt;

		while (true) {
			//cerr << "iter" << '\n';

			auto [lo_0, hi_0] = layout.get_node_bounds(a_0);
			auto [lo_1, hi_1] = layout.get_node_bounds(a_1);

			if (a_0 < layout.N && !cnd0) {
				cnd0 = v0 ? do_add(*v0, seg[0][a_0.c(0)]) : seg[0][a_0.c(0)];
			}
			if (a_1 < layout.N && !cnd1) {
				cnd1 = v1 ? do_add(*v1, seg[1][a_1.c(0)]) : seg[1][a_1.c(0)];
			}
			//cerr << " " << lo_0 << '-' << hi_0 << ' ' << a_0 << ' ' << v0.v << ' ' << (cnd0 ? cnd0->v : -1) << '\n';
			//cerr << " " << lo_1 << '-' << hi_1 << ' ' << a_1 << ' ' << v1.v << ' ' << (cnd1 ? cnd1->v : -1) << '\n';
			if (a_0 >= layout.N) {
				int idx = layout.get_node_split(a_1);
				if (idx + lo_0 == k) {
					return *cnd1;
				} else if (idx + lo_0 < k) {
					v1 = *cnd1;
					cnd1 = nullopt;
					a_1 = a_1.c(1);
				} else {
					cnd1 = nullopt;
					a_1 = a_1.c(0);
				}
			} else if (a_1 >= layout.N) {
				int idx = layout.get_node_split(a_0);
				if (idx + lo_1 == k) {
					return *cnd0;
				} else if (idx + lo_1 < k) {
					v0 = *cnd0;
					cnd0 = nullopt;
					a_0 = a_0.c(1);
				} else {
					cnd0 = nullopt;
					a_0 = a_0.c(0);
				}
			} else {
				int idx0 = layout.get_node_split(a_0);
				int idx1 = layout.get_node_split(a_1);
				if (idx0 + idx1 <= k) {
					//cerr << "drop smaller" << '\n';
					if (do_min(*cnd0, *cnd1) == *cnd0) {
						v0 = *cnd0;
						cnd0 = nullopt;
						a_0 = a_0.c(1);
					} else {
						v1 = *cnd1;
						cnd1 = nullopt;
						a_1 = a_1.c(1);
					}
				} else {
					//cerr << "drop bigger" << '\n';
					if (do_min(*cnd0, *cnd1) != *cnd0) {
						cnd0 = nullopt;
						a_0 = a_0.c(0);
					} else {
						cnd1 = nullopt;
						a_1 = a_1.c(0);
					}
				}
			}
		}
	};

	std::vector<uint64_t> answers; answers.reserve(Q);
	for (auto [type, a, b, c] : queries) {
		if (type == 1) {
			auto pt = layout.get_point(b);
			seg[a][pt] = c;
			pt.for_parents_up([&](auto x) -> void {
				update_node(a, x);
			});
		} else if (type == 2) {
			answers.push_back(solve(a));
		} else assert(false);
	}

	cout << '!' << ' ' << answers.size() << '\n';
	for (int i = 0; i < int(answers.size()); i++) {
		cout << answers[i] << " \n"[i+1==int(answers.size())];
	}
	cout << flush;

	return 0;
}
