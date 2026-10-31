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

	int N, M; cin >> N >> M;
	std::vector<std::array<int, 2>> vals(M);
	for (auto& [l, r] : vals) {
		cin >> l >> r;
		assert(1 <= l && l <= r && r <= N);
	}
	/*
	int N = int(1e6);
	int M = int(1e6);
	std::vector<std::array<int, 2>> vals(M);
	std::mt19937 rng(48);
	std::uniform_int_distribution<int> dist(1, N);
	for (int i = 0; i < M; i++) {
		int x = dist(rng);
		int y = dist(rng);
		if (x > y) std::swap(x, y);
		vals[i] = {x, y};
	}
	*/

	// We are guaranteed that either people <= R or lambda <= R
	int R = 0;
	while ((R+1) * (R+1) <= M) R++;

	seg_tree::in_order_layout layout(N+2);
	std::vector<std::vector<int>> by_right(N+1);
	for (auto [l, r] : vals) {
		by_right[r].push_back(l);
	}

	std::vector<int> ans(N+1, M+1);

	// min-plus seg tree
	struct seg_t {
		int val = 0;
		int lazy = 0;
	};
	std::vector<seg_t> seg;
	auto update_node = [&](seg_tree::point a) -> void {
		seg[a].val = std::min(seg[a.c(0)].val, seg[a.c(1)].val) + seg[a].lazy;
	};
	auto incr_node = [&](seg_tree::point a, int v) -> void {
		seg[a].val += v;
		seg[a].lazy += v;
	};
	auto downdate_node = [&](seg_tree::point a) -> void {
		if (!seg[a].lazy) return;
		incr_node(a.c(0), seg[a].lazy);
		incr_node(a.c(1), seg[a].lazy);
		seg[a].lazy = 0;
	};
	auto reset = [&]() -> void {
		seg.assign(2*layout.N, seg_t{M+1, 0});
	};
	auto add_range = [&](int l) -> void {
		// increment 0..l
		auto rng = layout.get_range(0, l);
		rng.for_each([&](seg_tree::point a) -> void {
			incr_node(a, 1);
		});
		rng.for_parents_up(update_node);
	};
	auto query = [&](int r) -> int {
		return seg[1].val;
		/*
		auto rng = layout.get_range(0, r);
		rng.for_parents_down(downdate_node);
		int res = M+1;
		rng.for_each([&](seg_tree::point a) -> void {
			res = std::min(res, seg[a].val);
		});
		return res;
		*/
	};
	auto set_value = [&](int r, int v) -> void {
		auto pt = layout.get_point(r);
		incr_node(pt, v - (M+1));
		pt.for_parents_up(update_node);
	};
	std::vector<int> cur_dp(N+2, M+1);
	cur_dp[0] = 0;
	for (int num_people = 0; num_people <= R; num_people++) {
		reset();
		set_value(0, 0);
		for (int i = 1; i <= N+1; i++) {
			for (int l : by_right[i-1]) {
				add_range(l);
			}
			int v = query(i);
			// TODO: set this en masse
			set_value(i, cur_dp[i]);
			cur_dp[i] = v;
		}
		ans[num_people] = std::min(ans[num_people], cur_dp[N+1]);
		cerr << "num_people " << num_people << ' ' << ans[num_people] << '\n';
	}

	std::vector<int> hull(R+1);
	for (int lambda = 0; lambda <= R; lambda++) {
		reset();
		set_value(0, 0);
		int tot = M+1;
		for (int i = 1; i <= N+1; i++) {
			for (int l : by_right[i-1]) {
				add_range(l);
			}
			int v = query(i);
			if (i == N+1) {
				tot = v;
			} else {
				set_value(i, v + lambda);
			}
		}
		assert(tot <= M);
		hull[lambda] = tot;
		cerr << "lambda " << lambda << ' ' << hull[lambda] << '\n';
	}

	for (int num_people = N, cur_lambda = 0; num_people > R; num_people--) {
		while (cur_lambda + 1 <= R && hull[cur_lambda + 1] - hull[cur_lambda] > num_people) {
			cur_lambda++;
		}
		ans[num_people] = hull[cur_lambda] - cur_lambda * num_people;
	}

	for (int i = 1; i <= N; i++) {
		cout << M - ans[i] << '\n';
	}

	return 0;
}
