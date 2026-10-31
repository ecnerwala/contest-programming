#include <bits/stdc++.h>

template <typename T> void setmax(T &a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T &a, T b) { if (b < a) a = b; }

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

seg_tree::in_order_layout layout;

const int INF = int(1e9) + 20;

// range set, point query
struct max_seg_tree {
	std::vector<int> v;
	max_seg_tree() : v(layout.N * 2, -INF) {}
	// inclusive update range
	void update(int l, int r, int x) {
		layout.get_range(l, r+1).for_each([&](auto a) {
			v[a] = std::max(v[a], x);
		});
	}
	int query(int a) {
		int res = -INF;
		if (a < 0 || a >= layout.N) return res;
		layout.get_point(a).for_each([&](auto a) {
			res = std::max(res, v[a]);
		});
		return res;
	}
};

struct min_seg_tree {
	std::vector<int> v;
	min_seg_tree() : v(layout.N * 2, +INF) {}
	// inclusive update range
	void update(int l, int r, int x) {
		layout.get_range(l, r+1).for_each([&](auto a) {
			v[a] = std::min(v[a], x);
		});
	}
	int query(int a) {
		int res = INF;
		if (a < 0 || a >= layout.N) return res;
		layout.get_point(a).for_each([&](auto a) {
			res = std::min(res, v[a]);
		});
		return res;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	std::vector<std::array<std::array<int, 2>, 2>> rects(N);
	for (auto& r : rects) {
		cin >> r[0][0] >> r[1][0] >> r[0][1] >> r[1][1];
	}

	std::vector<int> x_coords; x_coords.reserve(N);
	std::vector<int> y_coords; y_coords.reserve(N);
	for (int i = 0; i < N; i++) {
		x_coords.push_back(rects[i][0][0]);
		y_coords.push_back(rects[i][1][0]);
	}
	std::sort(x_coords.begin(), x_coords.end());
	std::sort(y_coords.begin(), y_coords.end());
	for (int i = 0; i < N; i++) {
		rects[i][0][0] = int(std::lower_bound(x_coords.begin(), x_coords.end(), rects[i][0][0]) - x_coords.begin());
		rects[i][0][1] = int(std::upper_bound(x_coords.begin(), x_coords.end(), rects[i][0][1]) - x_coords.begin()) - 1;
		rects[i][1][0] = int(std::lower_bound(y_coords.begin(), y_coords.end(), rects[i][1][0]) - y_coords.begin());
		rects[i][1][1] = int(std::upper_bound(y_coords.begin(), y_coords.end(), rects[i][1][1]) - y_coords.begin()) - 1;

		//cerr << rects[i][0][0] << '-' << rects[i][0][1] << " x " << rects[i][1][0] << '-' << rects[i][1][1] << '\n';
	}

	layout = seg_tree::in_order_layout(N);

	auto build_lims = [&](std::vector<std::array<int, 2>> p) -> std::array<std::array<int, 2>, 2> {
		std::array<std::array<int, 2>, 2> lims{std::array<int, 2>{-INF, INF}, {-INF, INF}};
		for (int i = 0; i < N; i++) {
			bool is_covered = false;
			for (auto [x, y] : p) {
				if (rects[i][0][0] <= x && x <= rects[i][0][1] && rects[i][1][0] <= y && y <= rects[i][1][1]) {
					is_covered = true;
					break;
				}
			}
			if (is_covered) continue;
			for (int d = 0; d < 2; d++) {
				lims[d][0] = std::max(lims[d][0], rects[i][d][0]);
				lims[d][1] = std::min(lims[d][1], rects[i][d][1]);
			}
		}
		/*
		cerr << "lims from " << '\n';
		for (auto [x, y] : p) {
			cerr << x << ' ' << y << '\n';
		}
		for (auto [x, y] : lims) { cerr << x << '-' << y << ' '; } cerr << '\n';
		*/
		return lims;
	};

	auto solve_1 = [&](std::vector<std::array<int, 2>> p) -> std::vector<std::array<int, 2>> {
		assert(M >= 1);
		auto lims = build_lims(p);
		if (lims[0][0] <= lims[0][1] && lims[1][0] <= lims[1][1]) {
			p.push_back({lims[0][0], lims[1][0]});
			return p;
		}
		return {};
	};

	auto solve_2 = [&](std::vector<std::array<int, 2>> p) -> std::vector<std::array<int, 2>> {
		assert(M >= 2);
		auto lims = build_lims(p);
		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				std::vector<std::array<int, 2>> np = p;
				np.push_back({lims[0][a], lims[1][b]});
				auto resp = solve_1(np);
				if (!resp.empty()) return resp;
			}
		}
		return {};
	};

	auto solve_3 = [&](std::vector<std::array<int, 2>> p) -> std::vector<std::array<int, 2>> {
		assert(M >= 3);
		auto lims = build_lims(p);
		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				std::vector<std::array<int, 2>> np = p;
				np.push_back({lims[0][a], lims[1][b]});
				auto resp = solve_2(np);
				if (!resp.empty()) return resp;
			}
		}
		return {};
	};

	auto solve_4 = [&]() -> std::vector<std::array<int, 2>> {
		assert(M >= 4);
		auto lims = build_lims({});
		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				std::vector<std::array<int, 2>> np;
				np.push_back({lims[0][a], lims[1][b]});
				auto resp = solve_3(np);
				if (!resp.empty()) return resp;
			}
		}

		// There must be a solution here
		// If we haven't found it, we can't be in the 1D case, since the greedy works for 1D
		assert(lims[0][0] > lims[0][1]);
		assert(lims[1][0] > lims[1][1]);

		int x0 = lims[0][1];
		int x1 = lims[0][0];

		int y0 = lims[1][1];
		int y1 = lims[1][0];

		min_seg_tree y0_x1_hi;
		max_seg_tree y1_x1_lo;

		max_seg_tree ys_lo;
		min_seg_tree ys_hi;

		max_seg_tree x0_x1_lo;
		min_seg_tree x0_x1_hi;

		min_seg_tree x0_y0_hi;
		min_seg_tree x0_y1_hi;

		cerr << "solving " << x0 << ' ' << x1 << ' ' << y0 << ' ' << y1 << '\n';

		int x0_lo = y0+1;
		int x0_hi = y1-1;

		std::vector<std::vector<std::pair<int, int>>> evts(N);

		for (int i = 0; i < N; i++) {
			auto [xs, ys] = rects[i];
			auto [xlo, xhi] = xs;
			auto [ylo, yhi] = ys;
			int num_cover = (xlo <= x0) + (x1 <= xhi) + (ylo <= y0) + (y1 <= yhi);
			assert(num_cover > 0);
			if (num_cover >= 3) {
				continue;
			} else if (num_cover == 1) {
				//cerr << "got 1 cover " << '\n';
				if (xlo <= x0) {
					setmax(x0_lo, ylo);
					setmin(x0_hi, yhi);
				} else if (x1 <= xhi) {
					x0_x1_lo.update(0, N-1, ylo);
					x0_x1_hi.update(0, N-1, yhi);
				} else if (ylo <= y0) {
					x0_y0_hi.update(0, N-1, xhi);
					y0_x1_hi.update(0, xlo-1, -1);
				} else if (y1 <= yhi) {
					x0_y1_hi.update(0, N-1, xhi);
					y1_x1_lo.update(0, xlo-1, N);
				} else assert(false);
			} else if (num_cover == 2) {
				//cerr << "got 2 cover " << '\n';
				if (xlo <= x0 && ylo <= y0) {
					x0_y0_hi.update(yhi+1, N-1, xhi);
				} else if (xlo <= x0 && y1 <= yhi) {
					x0_y1_hi.update(0, ylo-1, xhi);
				} else if (xlo <= x0 && x1 <= xhi) {
					x0_x1_lo.update(0, ylo-1, ylo);
					x0_x1_hi.update(0, ylo-1, yhi);

					x0_x1_lo.update(yhi+1, N-1, ylo);
					x0_x1_hi.update(yhi+1, N-1, yhi);
				} else if (ylo <= y0 && y1 <= yhi) {
					ys_lo.update(0, xlo, xlo);
					ys_hi.update(0, xlo, xhi);
				} else if (ylo <= y0 && x1 <= xhi) {
					y0_x1_hi.update(0, xlo-1, yhi);
				} else if (y1 <= yhi && x1 <= xhi) {
					y1_x1_lo.update(0, xlo-1, ylo);
				} else assert(false);
			} else assert(false);
		}

		for (int x0y = x0_lo; x0y <= x0_hi; x0y++) {
			//cerr << "trying " << x0 << ' ' << x0y << '\n';

			int y0_hi = x0_y0_hi.query(x0y);
			setmin(y0_hi, N-1);
			if (y0_hi < 0) continue;

			int y1_hi = x0_y1_hi.query(x0y);
			if (y1_hi < 0) continue;
			setmin(y1_hi, N-1);

			// pick the first thing that ends
			int y_first_hi = ys_hi.query(0);
			if (y_first_hi < 0) continue;
			setmin(y_first_hi, N-1);
			//cerr << "have " << y0_hi << ' ' << y1_hi << ' ' << y_first_hi << '\n';
			for (int z = 0; z < 2; z++) {
				int y0x, y1x;
				if (z == 0) {
					y0x = std::min(y0_hi, y_first_hi);
					y1x = std::min(y1_hi, ys_hi.query(y0x+1));
					if (y1x < 0) continue;
					setmin(y1x, N-1);
					if (y1x < ys_lo.query(y0x+1)) continue;
				} else {
					y1x = std::min(y1_hi, y_first_hi);
					y0x = std::min(y0_hi, ys_hi.query(y1x+1));
					if (y0x < 0) continue;
					setmin(y0x, N-1);
					if (y0x < ys_lo.query(y1x+1)) continue;
				}
				//cerr << "y0x " << y0x << ' ' << "y1x" << ' ' << y1x << '\n';
				int x1_lo = std::max(y1_x1_lo.query(y1x), x0_x1_lo.query(x0y));
				int x1_hi = std::min(y0_x1_hi.query(y0x), x0_x1_hi.query(x0y));
				//cerr << "x1y must be " << x1_lo << '-' << x1_hi << '\n';
				setmax(x1_lo, 0);
				setmin(x1_hi, N-1);
				if (x1_lo > x1_hi) continue;
				return {{x0, x0y}, {y0x, y0}, {y1x, y1}, {x1, x1_lo}};
			}
		}
		assert(false);
	};

	auto solve = [&]() -> std::vector<std::array<int, 2>> {
		std::vector<std::array<int, 2>> r;
		r = solve_1({});
		if (!r.empty()) return r;
		r = solve_2({});
		if (!r.empty()) return r;
		r = solve_3({});
		if (!r.empty()) return r;
		r = solve_4();
		if (!r.empty()) return r;
		assert(false);
	};
	auto ans = solve();
	cout << ans.size() << '\n';
	for (auto [x, y] : ans) {
		cout << x_coords[x] << ' ' << y_coords[y] << '\n';
	}

	return 0;
}
