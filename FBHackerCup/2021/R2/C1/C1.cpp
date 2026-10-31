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
	explicit point(int a_) : a(a_) { assert(a >= 0); }

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

	// Iterate over the range from outside-in.
	//   Calls f(point a, bool is_right)
	template <typename F> void for_each(F f) const {
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
		cerr << "Case #" << case_num << '\n';

		int L, N, K; cin >> L >> N >> K; K--;

		int cnt_over_K = 0;
		int cnt_over_L_K = 0;

		seg_tree::circular_layout layout(L);
		struct seg_node {
			int lazy = 0;
			int val = 0;
		};
		std::vector<seg_node> seg(2*L);
		auto update_node = [&](seg_tree::point a) -> void {
			seg[a].val = min(seg[a[0]].val, seg[a[1]].val) + seg[a].lazy;
		};
		auto add_delta = [&](int l, int r, int v) -> void {
			auto rng = layout.get_range(l, r);

			rng.for_each([&](seg_tree::point a, bool) -> void {
				seg[a].lazy += v;
				seg[a].val += v;
			});

			rng.for_parents_up(update_node);
		};
		auto incr = [&](int l, int r) -> void {
			add_delta(l, r, +1);
		};
		auto decr = [&](int l, int r) -> void {
			add_delta(l, r, -1);
		};

		for (int i = 0; i < L; i++) {
			add_delta(i, i+1, abs(i-K));
		}

		auto query = [&]() {
			int res = seg[1].val;
			res = min(res, L-K + cnt_over_K);
			res = min(res, K+1 + cnt_over_L_K);
			return res;
		};

		struct col_t {
			std::set<int> pts;
			std::set<pair<int, int>> lo_ranges;
			std::set<pair<int, int>> mid_ranges;
			std::set<pair<int, int>> hi_ranges;
			int sz() const {
				return int(pts.size());
			}
		};

		auto del_range = [&](col_t& col, int lo, int hi) {
			pair<int, int> r(lo, hi);
			if (col.lo_ranges.count(r)) {
				col.lo_ranges.erase(r);
				decr(lo+1, hi);
			} else if (col.mid_ranges.count(r)) {
				col.mid_ranges.erase(r);
			} else if (col.hi_ranges.count(r)) {
				col.hi_ranges.erase(r);
				decr(lo+1, hi);
			} else assert(false);

			if (!col.lo_ranges.empty() && int(col.mid_ranges.size() + col.hi_ranges.size()) < L-K) {
				auto it = std::prev(col.lo_ranges.end());
				decr(it->first+1, it->second);
				col.mid_ranges.insert(*it);
				col.lo_ranges.erase(it);
			}
			if (!col.hi_ranges.empty() && int(col.mid_ranges.size() + col.lo_ranges.size()) < K+1) {
				auto it = col.hi_ranges.begin();
				decr(it->first+1, it->second);
				col.mid_ranges.insert(*it);
				col.hi_ranges.erase(it);
			}
		};
		auto add_range = [&](col_t& col, int lo, int hi) {
			pair<int, int> r(lo, hi);
			if (col.mid_ranges.empty()) {
				assert(col.hi_ranges.empty());
				assert(col.lo_ranges.empty());
			}
			col.mid_ranges.insert(r);
			if (int(col.mid_ranges.size() + col.hi_ranges.size()) > L-K) {
				auto it = col.mid_ranges.begin();
				incr(it->first+1, it->second);
				col.lo_ranges.insert(*it);
				col.mid_ranges.erase(it);
			}
			if (int(col.mid_ranges.size() + col.lo_ranges.size()) > K+1) {
				auto it = std::prev(col.mid_ranges.end());
				incr(it->first+1, it->second);
				col.hi_ranges.insert(*it);
				col.mid_ranges.erase(it);
			}
		};

		auto add_pt = [&](col_t& col, int v) {
			incr(v, v+1);
			auto it = col.pts.insert(v).first;
			int prv = it == col.pts.begin() ? -1 : *std::prev(it);
			int nxt = std::next(it) == col.pts.end() ? L : *std::next(it);
			del_range(col, prv, nxt);
			add_range(col, prv, v);
			add_range(col, v, nxt);
		};
		auto del_pt = [&](col_t& col, int v) {
			decr(v, v+1);
			auto it = col.pts.find(v);
			int prv = it == col.pts.begin() ? -1 : *std::prev(it);
			int nxt = std::next(it) == col.pts.end() ? L : *std::next(it);
			col.pts.erase(v);
			del_range(col, prv, v);
			del_range(col, v, nxt);
			add_range(col, prv, nxt);
		};
		auto toggle_pt = [&](col_t& col, int v) {
			cnt_over_K -= col.sz() >= K+1;
			cnt_over_L_K -= col.sz() >= L-K;

			if (col.pts.count(v)) {
				del_pt(col, v);
			} else {
				add_pt(col, v);
			}

			cnt_over_K += col.sz() >= K+1;
			cnt_over_L_K += col.sz() >= L-K;
		};

		std::vector<col_t> cols(N);
		for (auto& col : cols) {
			add_range(col, -1, L);
		}

		for (int j = 0; j < L; j++) {
			string s; cin >> s;
			assert(int(s.size()) == N);
			for (int i = 0; i < N; i++) {
				assert(s[i] == 'X' || s[i] == '.');
				if (s[i] == 'X') {
					toggle_pt(cols[i], j);
				}
			}
		}

		for (int i = 0; i < L; i++) {
		}

		int ans = query();
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
