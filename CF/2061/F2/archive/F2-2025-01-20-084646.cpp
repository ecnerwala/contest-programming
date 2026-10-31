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

int solve(std::string S, std::string T) {
	T = char(S.front() ^ 1) + T + char(S.back() ^ 1);
	S = char(S.front() ^ 1) + S + char(S.back() ^ 1);

	int N = int(S.size());
	std::vector<std::array<int, 2>> nxt_idx(N);
	nxt_idx[N-1] = {N, N};
	for (int i = N-1; i >= 0; i--) {
		nxt_idx[i] = i == N-1 ? std::array<int, 2>{N, N} : nxt_idx[i+1];
		nxt_idx[i][S[i] - '0'] = i;
	}
	std::vector<std::array<int, 2>> prv_idx(N);
	for (int i = 0; i < N; i++) {
		prv_idx[i] = i == 0 ? std::array<int, 2>{-1, -1} : prv_idx[i-1];
		prv_idx[i][S[i] - '0'] = i;
	}
	// inclusive ranges
	std::vector<std::array<int, 2>> runs; runs.reserve(N);
	for (int i = 0, j = 0; i < N; i = j) {
		while (j < N && S[i] == S[j]) j++;
		runs.push_back({i, j-1});
	}
	int R = int(runs.size());
	std::vector<int> runs_pref(R+2);
	for (int i = 0; i < R; i++) {
		runs_pref[i+2] = runs_pref[i] + (runs[i][1] - runs[i][0] + 1);
	}

	assert((runs.front() == std::array<int, 2>{0, 0}));
	assert((runs.back() == std::array<int, 2>{N-1, N-1}));
	std::vector<int> dp(R, -1);

	dp[0] = 1;

	seg_tree::in_order_layout layout(R);
	std::vector<std::array<int, 2>> seg(2*R, {-1, -1});
	auto update_node = [&](seg_tree::point p) -> void {
		seg[p][0] = std::max(seg[p.c(0)][0], seg[p.c(1)][0]);
		seg[p][1] = std::max(seg[p.c(0)][1], seg[p.c(1)][1]);
	};

	std::vector<std::vector<int>> evts(R);

	for (int r = 0; r < R; r++) {
		auto [a, b] = runs[r];
		int v = S[a] - '0';
		if (nxt_idx[a][!v] <= b) {
			// bad range
			dp[r] = -1;
			continue;
		}

		// query the previous
		if (r > 0) {
			int prv_bad = prv_idx[b][!v];
			assert(prv_bad < a);
			int max_cnt = b - prv_bad;
			// number of regions of the same color to include
			int mi = 0;
			int ma = (r+2) / 2 + 1;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (runs_pref[r+2] - runs_pref[r + 2 - (2 * md)] <= max_cnt) {
					mi = md;
				} else {
					ma = md;
				}
			}
			assert(mi >= 1);
			// query from runs since r - (mi - 1) * 2
			int lo = r - mi * 2 + 1;
			lo = std::max(lo, 0);
			int best = -1;
			layout.get_range(lo, r).for_each([&](auto p) -> void {
				best = std::max(best, seg[p][!v]);
			});
			if (best >= 1) best++;
			dp[r] = best;
		}

		for (auto e : evts[r]) {
			auto p = layout.get_point(e);
			seg[p] = {-1, -1};
			p.for_parents_up(update_node);
		}

		if (r < R-1) {
			int nxt_bad = nxt_idx[a][!v];
			assert(nxt_bad > b);
			int max_cnt = nxt_bad - a;
			int mi = 0;
			int ma = (R - r + 1) / 2 + 1;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (runs_pref[r + 2 * md] - runs_pref[r] <= max_cnt) {
					mi = md;
				} else {
					ma = md;
				}
			}
			assert(mi >= 1);
			int hi = r + mi * 2 - 1;
			hi = std::min(hi, R-1);
			auto p = layout.get_point(r);
			seg[p][v] = dp[r];
			p.for_parents_up(update_node);
			assert(hi > r);
			evts[hi].push_back(r);
		}
	}

	return dp[R-1] == -1 ? -1 : R - dp[R-1];
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		std::string S, T; cin >> S >> T;
		cout << solve(S, T) << '\n';
	}

	return 0;
}
