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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		//cerr << "solving " << N << '\n';
		//for (int i = 0; i < N; i++) { cerr << A[i] << " \n"[i+1==N]; }

		std::vector<std::pair<int, int>> vals(N);
		for (int i = 0; i < N; i++) {
			vals[i] = {A[i], i};
		}
		std::sort(vals.begin(), vals.end(), std::greater<>());

		std::vector<std::pair<int, std::array<int, 2>>> intervals;
		intervals.reserve(N);

		std::map<int, int> cur_pareto;
		cur_pareto[N] = N;

		for (int i = 0, j = 0; i < N; i = j) {
			int v = vals[i].first;
			while (j < N && vals[j].first == v) j++;
			assert(j > i);
			int r = vals[i].second;
			int l = vals[j-1].second;
			assert(l <= r);
			auto it = cur_pareto.lower_bound(l);
			assert(it->first != l);
			assert(it->first > l);
			if (it->second <= r) {
				// this interval is bad
				continue;
			} else {
				intervals.push_back({v, {l, r}});
				it = cur_pareto.insert(it, {l, r});
				while (it != cur_pareto.begin() && std::prev(it)->second >= r) {
					cur_pareto.erase(std::prev(it));
				}
			}
		}

		std::vector<int> B(N, -1);
		seg_tree::in_order_layout layout(N);
		std::vector<int> seg(2*N, -1);
		for (auto [v_, inter] : intervals) {
			int v = v_;
			auto [l, r] = inter;
			assert(B[l] == -1 && B[r] == -1);
			B[l] = B[r] = v;
			auto rng = layout.get_range(l, r + 1);
			rng.for_each([&](seg_tree::point a) { seg[a] = std::max(seg[a], v); });
		}
		for (seg_tree::point a(1); a < N; a++) {
			seg[a.c(0)] = std::max(seg[a.c(0)], seg[a]);
			seg[a.c(1)] = std::max(seg[a.c(1)], seg[a]);
		}

		const int INF = (*std::max_element(A.begin(), A.end())) + 1;

		std::vector<int> sorted_bests; sorted_bests.reserve(N);

		std::vector<int> B_maxes(N);
		bool has_unfilled = false;
		for (int i = 0; i < N; i++) {
			if (B[i] == -1) {
				B_maxes[i] = seg[layout.get_point(i)];
				if (B_maxes[i] == -1) {
					has_unfilled = true;
				}
				sorted_bests.push_back(B_maxes[i]);
			} else {
				B_maxes[i] = INF;
			}
		}

		//for (int i = 0; i < N; i++) { cerr << B[i] << " \n"[i+1==N]; }
		//for (int i = 0; i < N; i++) { cerr << B_maxes[i] << " \n"[i+1==N]; }

		std::sort(sorted_bests.begin(), sorted_bests.end());

		std::vector<int> pref_best(N+1);
		pref_best[0] = INF;
		for (int i = 0; i < N; i++) {
			pref_best[i+1] = std::min(pref_best[i], B_maxes[i]);
		}
		std::vector<int> suff_best(N+1);
		suff_best[N] = INF;
		for (int i = N-1; i >= 0; i--) {
			suff_best[i] = std::min(suff_best[i+1], B_maxes[i]);
		}

		struct sol_t {
			int64_t cost;
			int mex;
			int vl;
			int vr;
			std::array<int, 2> bounds;
		};
		std::optional<sol_t> best_sol;
		if (!has_unfilled) {
			best_sol = sol_t{
				0, -1, -1, -1, {-1, -1},
			};
		}

		{
			auto sb_it = sorted_bests.begin();
			int cnt_smaller = 0;
			int64_t sum_smaller = 0;

			std::reverse(intervals.begin(), intervals.end());
			int mex = 0;
			int last_v = -1;
			for (auto [v, bounds] : intervals) {
				assert(v > last_v);
				if (v - last_v > 1) {
					mex = v - 1;
				} else {
					// mex doesn't change
				}
				last_v = v;

				assert(mex >= 0);
				if (mex == 0) continue;

				for (; sb_it != sorted_bests.end() && *sb_it <= mex; ++sb_it) {
					cnt_smaller++;
					sum_smaller += *sb_it;
				}

				int vl = pref_best[bounds[0]];
				int vr = suff_best[bounds[1] + 1];
				if (vl == INF || vr == INF) continue;

				int64_t cost = int64_t(cnt_smaller) * int64_t(mex) - int64_t(sum_smaller);
				if (vl > mex) {
					cost += mex;
					cost -= vl;
				}
				if (vr > mex) {
					cost += mex;
					cost -= vr;
				}

				sol_t cur_sol{
					cost, mex, vl, vr, bounds,
				};

				if (!best_sol || cost > best_sol->cost) {
					best_sol = cur_sol;
				}
			}
		}

		assert(best_sol);

		sol_t sol = *best_sol;
		if (sol.mex == -1) {
			assert(sol.cost == 0);
			assert(!has_unfilled);
			for (int i = 0; i < N; i++) {
				if (B[i] == -1) {
					B[i] = B_maxes[i];
				}
			}
		} else {
			assert(sol.mex > 0);
			//cerr << sol.mex << '\n';
			int mex = sol.mex;
			for (int i = 0; i < N; i++) {
				if (B[i] == -1) {
					assert(B_maxes[i] != mex);
					if (B_maxes[i] < mex) {
						B[i] = mex;
					} else if (i < sol.bounds[0] && B_maxes[i] == sol.vl) {
						B[i] = mex;
						sol.vl = mex;
					} else if (i > sol.bounds[1] && B_maxes[i] == sol.vr) {
						B[i] = mex;
						sol.vr = mex;
					} else {
						B[i] = B_maxes[i];
					}
				}
			}
		}

		for (int i = 0; i < N; i++) {
			cout << B[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
