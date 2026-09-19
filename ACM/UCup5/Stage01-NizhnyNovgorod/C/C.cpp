#include <bits/stdc++.h>
#include <cassert>

namespace wala::seg_tree {

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

} // namespace wala::seg_tree

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N, Q; std::cin >> N >> Q;
	std::vector<int> A(N); for (auto& a : A) std::cin >> a;

	using dbl = double;
	const dbl MAX_VAL = 1e9;
	const dbl MAX_LOG = std::log(dbl(MAX_VAL)) + 1;
	const dbl INF = 1e99;
	struct evt_t {
		std::array<int, 2> bounds;
		dbl v;
	};
	std::vector<std::vector<evt_t>> evts(N+1);
	for (int q = 0; q < Q; q++) {
		int l1, r1, l2, r2, c; std::cin >> l1 >> r1 >> l2 >> r2 >> c; l1--, l2--;
		dbl v = std::log(dbl(c));
		evts[l1].push_back({{l2, r2}, +v});
		evts[r1].push_back({{l2, r2}, -v});
		evts[l2].push_back({{l1, r1}, +v});
		evts[r2].push_back({{l1, r1}, -v});

		int la = std::max(l1, l2);
		int ra = std::min(r1, r2);
		if (la < ra) {
			evts[la].push_back({{la, ra}, -v});
			evts[ra].push_back({{la, ra}, +v});
		}
	}

	auto ans = [&]() -> dbl {
		int num_comps = N;
		dbl tot_ans = 0;
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		wala::seg_tree::in_order_layout layout(N);
		struct seg_node {
			std::array<std::pair<dbl, int>, 2> mins;
			dbl lazy;
		};
		std::vector<seg_node> seg(2 * layout.N);
		auto update_node = [&](wala::seg_tree::point a) -> void {
			seg[a].mins[0] = std::min(seg[a.c(0)].mins[0], seg[a.c(1)].mins[0]);
			int side = seg[a].mins[0].second;
			seg[a].mins[1] = std::min(
				seg[a.c(0)].mins[seg[a.c(0)].mins[0].second == side],
				seg[a.c(1)].mins[seg[a.c(1)].mins[0].second == side]
			);
			seg[a].mins[0].first += seg[a].lazy;
			seg[a].mins[1].first += seg[a].lazy;
		};
		auto do_update = [&](evt_t e) {
			std::cerr << "do update " << e.bounds[0] << '-' << e.bounds[1] << ' ' << std::exp(e.v) << '\n';
			auto rng = layout.get_range(e.bounds);
			rng.for_each([&](auto a) -> void {
				seg[a].mins[0].first += e.v;
				seg[a].mins[1].first += e.v;
				seg[a].lazy += e.v;
			});
			rng.for_parents_up(update_node);
		};
		std::vector<int> cur_comp(N);
		while (num_comps > 1) {
			std::cerr << "start iteration" << '\n';
			// reset the seg
			for (int i = 0; i < N; i++) {
				cur_comp[i] = get_par(i);
				std::cerr << i << ' ' << cur_comp[i] << '\n';
				auto a = layout.get_point(i);
				seg[a].mins = {std::pair<dbl, int>{std::log(dbl(A[i])) - std::log(dbl(A[0])), cur_comp[i]}, {INF, -1}};
				seg[a].lazy = 0;
			}
			for (wala::seg_tree::point a(layout.N - 1); a > 0; a--) {
				seg[a].lazy = 0;
				update_node(a);
			}

			for (int i = 0; i < N; i++) {
				if (i) {
					// move seg[a].mins
					dbl v = std::log(dbl(A[i])) - std::log(dbl(A[i-1]));
					do_update({{0, i}, +v});
					do_update({{i, N}, -v});
				}
				for (auto e : evts[i]) do_update(e);
				auto [cost, tgt] = seg[1].mins[seg[1].mins[0].second == cur_comp[i]];
				std::cerr << "merge " << i << ' ' << tgt << ' ' << std::exp(cost) << '\n';
				assert(tgt != -1);
				if (merge(i, tgt)) {
					std::cerr << "yes!" << '\n';
					num_comps--;
					if (cost >= MAX_LOG) return MAX_VAL;
					tot_ans += std::exp(cost);
				}
			}
		}
		return std::min(tot_ans, MAX_VAL);
	}();
	std::cout << std::fixed << std::setprecision(20) << ans << '\n';

	return 0;
}
