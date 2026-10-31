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
		auto [x, y] = get_node_bounds(pt);
		return y - x;
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
		auto [x, y] = get_node_bounds(pt);
		int r = y - x;
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<pair<int, int64_t>> A(N); for (auto& a : A) { cin >> a.first >> a.second; a.first--; }

	vector<int> cycle; cycle.reserve(N);
	vector<int> cycle_index(N, -1);
	{
		int st = 0;
		for (int z = 0; z < N; z++) st = A[st].first;
		while (cycle_index[st] == -1) {
			cycle_index[st] = int(cycle.size());
			cycle.push_back(st);
			st = A[st].first;
		}
	}

	vector<array<int, 20>> par(N);
	vector<array<int, 20>> marked(N);

	for (int i = 0; i < N; i++) {
		marked[i][0] = false;
		par[i][0] = cycle_index[i] != -1 ? -1 : A[i].first;
	}
	for (int l = 0; l+1 < 20; l++) {
		for (int i = 0; i < N; i++) {
			marked[i][l+1] = false;
			if (par[i][l] == -1) {
				par[i][l+1] = -1;
			} else {
				par[i][l+1] = par[par[i][l]][l];
			}
		}
	}

	vector<int> depth(N);
	for (int i = 0; i < N; i++) {
		depth[i] = 0;
		int cur = i;
		int l = 0;
		while (par[cur][l] != -1) l++;
		while (l > 0) {
			l--;
			if (par[cur][l] != -1) {
				depth[i] += (1 << l);
				cur = par[cur][l];
			}
		}
	}

	vector<vector<int>> cycle_edges(cycle.size());

	for (int e = 0; e < M; e++) {
		int x, y; cin >> x >> y; x--, y--;
		if (depth[y] < depth[x]) std::swap(x, y);
		for (int d = depth[y] - depth[x]; d > 0; d &= d-1) {
			int l = __builtin_ctz(d);
			marked[y][l] = true;
			y = par[y][l];
		}
		assert(depth[x] == depth[y]);
		if (x == y) continue;
		int l = 0;
		while (par[x][l] != par[y][l]) l++;
		while (l > 0) {
			l--;
			if (par[x][l] != par[y][l]) {
				marked[x][l] = true;
				marked[y][l] = true;
				x = par[x][l];
				y = par[y][l];
			}
		}
		assert(x != y && par[x][0] == par[y][0]);
		if (par[x][0] != -1) {
			marked[x][0] = marked[y][0] = true;
			continue;
		}
		assert(cycle_index[x] != -1);
		assert(cycle_index[y] != -1);
		x = cycle_index[x];
		y = cycle_index[y];
		cycle_edges[x].push_back(y);
		cycle_edges[y].push_back(x);
	}

	seg_tree::in_order_layout layout(int(cycle.size()));
	struct seg_data {
		int lazy_cnt = 0;
		int64_t tot = 0;
		int64_t val = 0;
	};
	vector<seg_data> seg(2*int(cycle.size()));
	auto update_node = [&](seg_tree::point a) -> void {
		if (seg[a].lazy_cnt) {
			seg[a].val = seg[a].tot;
		} else if (a >= int(cycle.size())) {
			seg[a].val = 0;
		} else {
			seg[a].val = seg[a[0]].val + seg[a[1]].val;
		}
	};
	for (int i = 0; i < int(cycle.size()); i++) {
		seg[layout.get_point(i)].tot = A[cycle[i]].second;
	}
	for (int i = int(cycle.size())-1; i; i--) {
		seg[i].tot = seg[2*i].tot + seg[2*i+1].tot;
	}

	auto mark_range = [&](int a, int b) -> void {
		auto r = layout.get_range(a, b);
		r.for_each([&](auto p, bool) -> void {
			seg[p].lazy_cnt++;
			update_node(p);
		});
		r.for_parents_up(update_node);
	};
	auto unmark_range = [&](int a, int b) -> void {
		auto r = layout.get_range(a, b);
		r.for_each([&](auto p, bool) -> void {
			seg[p].lazy_cnt--;
			update_node(p);
		});
		r.for_parents_up(update_node);
	};

	for (int i = 0; i < int(cycle.size()); i++) {
		for (int j : cycle_edges[i]) {
			if (j > i) {
				mark_range(i, j);
			}
		}
	}

	int64_t ans = seg[1].val;
	for (int i = 0; i < int(cycle.size()); i++) {
		for (int j : cycle_edges[i]) {
			if (j > i) {
				unmark_range(i, j);
				mark_range(0, i);
				mark_range(j, int(cycle.size()));
			} else {
				unmark_range(0, j);
				unmark_range(i, int(cycle.size()));
				mark_range(j, i);
			}
		}
		ans = min(ans, seg[1].val);
	}

	for (int l = 19; l > 0; l--) {
		for (int i = 0; i < N; i++) {
			if (marked[i][l]) {
				assert(par[i][l] != -1);
				marked[i][l-1] = true;
				marked[par[i][l-1]][l-1] = true;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		if (marked[i][0]) {
			assert(par[i][0] != -1);
			ans += A[i].second;
		}
	}

	cout << ans << '\n';

	return 0;
}
