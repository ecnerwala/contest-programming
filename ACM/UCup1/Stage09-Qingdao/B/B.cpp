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

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(1e5) + 10;
	seg_tree::in_order_layout layout(V);
	std::vector<std::array<int, 2>> seg(2*V);
	auto update_node = [&](seg_tree::point a) -> void {
		for (int z = 0; z < 2; z++) {
			seg[a][z] = std::max(seg[a.c(0)][z], seg[a.c(1)][z]);
		}
	};
	auto update_val = [&](int idx, std::array<int, 2> diff) -> void {
		auto a = layout.get_point(idx);
		for (int z = 0; z < 2; z++) {
			seg[a][z] += diff[z];
		}
		a.for_parents_up(update_node);
	};
	auto get_best = [&]() -> int {
		return seg[1][0] + seg[1][1];
	};

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, M; cin >> N >> M;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<std::vector<std::pair<int, int>>> full_adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			full_adj[u].push_back({e, v});
			full_adj[v].push_back({e, u});
		}

		int tot_ans = 0;
		std::vector<int> edge_ans(M, 0);

		std::vector<std::vector<std::pair<int, int>>> tree_ch(N);

		std::vector<int> sz(N);
		std::vector<int> depth(N, -1);
		std::vector<int> lowval(N, -1);

		std::vector<int> cur_chain; cur_chain.reserve(N);
		for (int rt = 0; rt < N; rt++) {
			if (depth[rt] != -1) continue;
			depth[rt] = 0;
			std::y_combinator([&](auto self, int cur, int prvE) -> void {
				sz[cur] = 1;
				lowval[cur] = depth[cur];
				for (auto [e, nxt] : full_adj[cur]) {
					if (e == prvE) continue;
					if (depth[nxt] == -1) {
						depth[nxt] = depth[cur] + 1;
						self(nxt, e);
						sz[cur] += sz[nxt];
						if (lowval[nxt] == depth[nxt]) {
							tree_ch[cur].push_back({e, nxt});
							//cerr << "tree " << cur << ' ' << -1 << ' ' << nxt << '\n';
						} else {
							lowval[cur] = std::min(lowval[cur], lowval[nxt]);

							tree_ch[cur].push_back({-1, nxt});
							//cerr << "tree " << cur << ' ' << -1 << ' ' << nxt << '\n';
						}
					} else if (depth[nxt] <= depth[cur]) {
						lowval[cur] = std::min(lowval[cur], depth[nxt]);
					} else {
						// down edge, ignore
					}
				}

				// set up the preorder-HLD
				for (int z = 1; z < int(tree_ch[cur].size()); z++) {
					if (sz[tree_ch[cur][z].second] > sz[tree_ch[cur][0].second]) {
						std::swap(tree_ch[cur][z], tree_ch[cur][0]);
					}
				}
				update_val(A[cur], {+1, 0});
			})(rt, -1);

			auto mark_node = [&](int v) -> void {
				update_val(A[v], {-1, +1});
			};
			auto mark_all = std::y_combinator([&](auto self, int v) -> void {
				mark_node(v);
				for (auto [_, u] : tree_ch[v]) {
					self(u);
				}
			});
			auto unmark_node = [&](int v) -> void {
				update_val(A[v], {+1, -1});
			};
			auto unmark_all = std::y_combinator([&](auto self, int v) -> void {
				unmark_node(v);
				for (auto [_, u] : tree_ch[v]) {
					self(u);
				}
			});

			int cc_val = get_best();
			tot_ans += cc_val;

			std::y_combinator([&](auto self, int cur)-> void {
				//cerr << "Outer " << cur << '\n';
				// handle + recurse into light edges
				for (int v = cur; !tree_ch[v].empty(); ) {
					//cerr << "Chain " << cur << ' ' << v << '\n';
					for (int z = 1; z < int(tree_ch[v].size()); z++) {
						auto [e, u] = tree_ch[v][z];
						self(u);
						mark_all(u);
						int res = get_best() - cc_val;
						if (e != -1) edge_ans[e] = res;
						unmark_all(u);
					}
					int u = tree_ch[v][0].second;
					v = u;
				}
				//cerr << "Solving outer " << cur << '\n';

				cur_chain.clear();
				cur_chain.push_back(cur);
				while (!tree_ch[cur_chain.back()].empty()) {
					cur_chain.push_back(tree_ch[cur_chain.back()][0].second);
				}
				std::y_combinator([&](auto self2, int l, int r) -> void {
					if (r - l == 1) {
						int v = cur_chain[l];
						if (tree_ch[v].empty()) return;
						int e = tree_ch[v][0].first;

						int res = get_best() - cc_val;
						if (e != -1) edge_ans[e] = res;
						return;
					}

					int m = l+1;
					int tgt = (r == int(cur_chain.size()) ? 0 : sz[cur_chain[r]]) + sz[cur_chain[l]];
					while (m < r && sz[cur_chain[m]] * 2 > tgt) {
						m++;
					}
					m--;
					m = std::min(std::max(m, l+1), r-1);
					assert(l < m && m < r);

					self2(m, r);

					// now go mark everything at m..r or deeper
					for (int x = m; x < r; x++) {
						int v = cur_chain[x];
						mark_node(v);
						for (int z = 1; z < int(tree_ch[v].size()); z++) {
							mark_all(tree_ch[v][z].second);
						}
					}
					self2(l, m);
					for (int x = m; x < r; x++) {
						int v = cur_chain[x];
						unmark_node(v);
						for (int z = 1; z < int(tree_ch[v].size()); z++) {
							unmark_all(tree_ch[v][z].second);
						}
					}
				})(0, int(cur_chain.size()));
			})(rt);

			std::y_combinator([&](auto self, int cur)-> void {
				update_val(A[cur], {-1, 0});
				for (auto [e, nxt] : tree_ch[cur]) {
					self(nxt);
				}
			})(rt);
		}

		for (int e = 0; e < M; e++) {
			cout << edge_ans[e] + tot_ans << " \n"[e+1==M];
		}
	}

	return 0;
}
