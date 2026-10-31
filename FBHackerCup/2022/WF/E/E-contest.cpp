#include<bits/stdc++.h>

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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> P(N);
		for (auto& [x, y] : P) {
			cin >> x >> y;
		}
		cerr << "Case #" << case_num << ": N = " << N << '\n';

		std::vector<int> xs(N);
		std::vector<int> ys(N);
		for (int i = 0; i < N; i++) {
			xs[i] = P[i][0] - 1;
			ys[i] = P[i][1] - 1;
		}
		std::sort(xs.begin(), xs.end());
		std::sort(ys.begin(), ys.end());

		std::vector<std::array<std::array<int, 2>, 2>> rects(N);
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};

		seg_tree::in_order_layout layout(N);
		std::vector<std::multimap<int, int>> x_pt_y_pt(2*N);
		std::vector<std::multimap<int, int>> x_rg_y_pt(2*N);
		std::vector<std::multimap<int, int>> y_rg_x_pt(2*N);
		std::vector<std::map<int, std::pair<int, int>>> x_rg_y_rg(2*N);

		for (int i = 0; i < N; i++) {
			rects[i][0] = {P[i][0] - 1, P[i][0] + 1};
			rects[i][1] = {P[i][1] - 1, P[i][1] + 1};

			bool did_change = true;
			while (did_change) {
				//cerr << "cur " << i << ' ' << rects[i][0][0] << '-' << rects[i][0][1] << ' ' << rects[i][1][0] << '-' << rects[i][1][1] << '\n';
				did_change = false;

				int xlo = int(std::lower_bound(xs.begin(), xs.end(), rects[i][0][0]) - xs.begin());
				int xhi = int(std::upper_bound(xs.begin(), xs.end(), rects[i][0][1]) - xs.begin())-1;
				int ylo = int(std::lower_bound(ys.begin(), ys.end(), rects[i][1][0]) - ys.begin());
				int yhi = int(std::upper_bound(ys.begin(), ys.end(), rects[i][1][1]) - ys.begin())-1;

				auto check = [&](int j) {
					j = get_par(j);
					if (j == i) return;

					assert(par[j] < 0);

					did_change = true;
					par[j] = i;
					for (int d = 0; d < 2; d++) {
						rects[i][d][0] = std::min(rects[i][d][0], rects[j][d][0]);
						rects[i][d][1] = std::max(rects[i][d][1], rects[j][d][1]);
					}
				};

				layout.get_range(xlo, xhi+1).for_each([&](auto a) {
					auto& s = x_pt_y_pt[a];
					auto it = s.lower_bound(ylo);
					while (it != s.end() && it->first <= yhi) {
						//cerr << " x_pt_y_pt check " << ' ' << it->second << '\n';
						check(it->second);
						it = s.erase(it);
					}
				});
				layout.get_point(xlo).for_each([&](auto a) {
					auto& s = x_rg_y_pt[a];
					auto it = s.lower_bound(ylo);
					while (it != s.end() && it->first <= yhi) {
						//cerr << " x_rg_y_pt check " << ' ' << it->second << '\n';
						check(it->second);
						it = s.erase(it);
					}
				});
				layout.get_point(ylo).for_each([&](auto a) {
					auto& s = y_rg_x_pt[a];
					auto it = s.lower_bound(xlo);
					while (it != s.end() && it->first <= xhi) {
						//cerr << " y_rg_x_pt check " << ' ' << it->second << '\n';
						check(it->second);
						it = s.erase(it);
					}
				});
				layout.get_point(xlo).for_each([&](auto a) {
					auto& s = x_rg_y_rg[a];
					auto it = s.lower_bound(ylo);
					if (it != s.end() && it->second.first <= ylo) {
						//cerr << " x_rg_y_rg check " << ' ' << it->second.second << '\n';
						check(it->second.second);
						it = s.erase(it);
					}
				});
			}
			//cerr << "final " << i << ' ' << rects[i][0][0] << '-' << rects[i][0][1] << ' ' << rects[i][1][0] << '-' << rects[i][1][1] << '\n';

			// insert the things
			{
				int xlo = int(std::lower_bound(xs.begin(), xs.end(), rects[i][0][0]) - xs.begin());
				int xhi = int(std::upper_bound(xs.begin(), xs.end(), rects[i][0][1]) - xs.begin())-1;
				int ylo = int(std::lower_bound(ys.begin(), ys.end(), rects[i][1][0]) - ys.begin());
				int yhi = int(std::upper_bound(ys.begin(), ys.end(), rects[i][1][1]) - ys.begin())-1;

				layout.get_point(xlo).for_each([&](auto a) {
					x_pt_y_pt[a].insert({ylo, i});
				});
				layout.get_range(xlo, xhi+1).for_each([&](auto a) {
					x_rg_y_pt[a].insert({ylo, i});
				});
				layout.get_range(ylo, yhi+1).for_each([&](auto a) {
					y_rg_x_pt[a].insert({xlo, i});
				});
				layout.get_range(xlo, xhi+1).for_each([&](auto a) {
					x_rg_y_rg[a].insert({yhi, {ylo, i}});
				});
			}
		}

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			if (par[i] < 0) {
				auto [xlo, xhi] = rects[i][0];
				auto [ylo, yhi] = rects[i][1];
				ans += int64_t(xhi - xlo) * int64_t(yhi - ylo);
			}
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
