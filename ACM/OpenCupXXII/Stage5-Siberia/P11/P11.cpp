#include <bits/stdc++.h>

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

	int N, Q; cin >> N >> Q;
	struct node_t {
		int depth = -1;
		int root = -1;
		bool is_cycle = false;
		int cyc_len = -1;
		int st = 0, en = 0;
		std::array<int, 20> jump;
	};
	auto read_tree = [&]() -> std::vector<node_t> {
		std::vector<node_t> G(N);

		std::vector<int> F(N);
		for (auto& f : F) { cin >> f; f--; }

		for (int st = 0; st < N; st++) {
			if (G[st].depth != -1) continue;

			int d = 0;
			int cur = st;
			while (G[cur].depth == -1) {
				G[cur].depth = -2-d;
				cur = F[cur];
				d++;
			}

			if (G[cur].depth < 0) {
				G[st].cyc_len = d - (-2 - G[cur].depth);
				G[st].depth = d - 1;

				for (int i = 0; i+1 < G[st].cyc_len; i++) {
					G[cur].is_cycle = true;
					cur = F[cur];
				}
				G[cur].is_cycle = true;

				G[st].root = cur;

				for (int z = 1, a = F[st]; z <= G[st].depth; z++, a = F[a]) {
					G[a].depth = G[st].depth - z;
					G[a].root = G[st].root;
					G[a].cyc_len = G[st].cyc_len;
				}
			} else {
				for (int a = st; a != cur; a = F[a]) {
					G[a].depth = G[cur].depth + d;
					d--;
					G[a].root = G[cur].root;
					G[a].cyc_len = G[cur].cyc_len;
				}
			}
		}

		for (int i = 0; i < N; i++) {
			G[i].jump[0] = F[i];
		}
		for (int z = 0; z+1 < 20; z++) {
			for (int i = 0; i < N; i++) {
				G[i].jump[z+1] = G[G[i].jump[z]].jump[z];
			}
		}

		std::vector<std::vector<int>> ch(N);
		for (int i = 0; i < N; i++) {
			ch[F[i]].push_back(i);
		}
		int cur_idx = 0;
		for (int rt = 0; rt < N; rt++) {
			assert((G[rt].root == rt) == (G[rt].depth == 0));
			if (G[rt].root == rt) {
				std::y_combinator([&](auto self, int cur) -> void {
					G[cur].st = cur_idx;
					cur_idx++;
					for (int nxt : ch[cur]) {
						if (nxt == rt) continue;
						self(nxt);
					}
					G[cur].en = cur_idx;
				})(rt);
			}
		}
		assert(cur_idx == N);

		return G;
	};

	std::vector<node_t> F = read_tree();
	std::vector<node_t> G = read_tree();

	std::vector<std::array<int, 2>> queries(Q);
	for (auto& q : queries) {
		cin >> q[0] >> q[1]; q[0]--, q[1]--;
	}

	std::vector<bool> ans(Q, false);

	{ // Both cycle
		std::set<std::pair<std::array<int, 2>, int>> both_cycle;
		for (int i = 0; i < N; i++) {
			if (F[i].is_cycle && G[i].is_cycle) {
				int l = std::gcd(F[i].cyc_len, G[i].cyc_len);
				int d = (F[i].depth - G[i].depth) % l;
				if (d < 0) d += l;
				both_cycle.insert({{F[i].root, G[i].root}, d});
			}
		}

		for (int q = 0; q < Q; q++) {
			auto [x, y] = queries[q];
			int l = std::gcd(F[x].cyc_len, G[y].cyc_len);
			int d = (F[x].depth - G[y].depth) % l;
			if (d < 0) d += l;
			if (both_cycle.count({{F[x].root, G[y].root}, d})) {
				ans[q] = true;
			}
		}
	}

	{ // Tree
		std::vector<std::vector<int>> by_depth(2*N+1);
		for (int i = 0; i < N; i++) {
			by_depth[F[i].depth - G[i].depth + N].push_back(i);
		}
		for (int q = 0; q < Q; q++) {
			auto [x, y] = queries[q];
			by_depth[F[x].depth - G[y].depth + N].push_back(N+q);
		}

		seg_tree::in_order_layout layout(N);
		std::vector<int> seg(2*N);

		std::vector<std::pair<int, int>> evts; evts.reserve(2*N+Q);
		for (const auto& v : by_depth) {
			for (auto cnd : v) {
				if (cnd < N) {
					int i = cnd;
					evts.push_back({F[i].st, cnd});
					evts.push_back({F[i].en, cnd});
				} else {
					int q = cnd - N;
					auto [x, y] = queries[q];
					evts.push_back({F[x].st, cnd});
				}
			}
			std::sort(evts.begin(), evts.end());
			for (auto [loc, cnd] : evts) {
				if (cnd < N) {
					int i = cnd;
					auto rng = layout.get_range(G[i].st, G[i].en);
					if (loc == F[i].st) {
						rng.for_each([&](auto a, bool) { seg[a]++; });
					} else if (loc == F[i].en) {
						rng.for_each([&](auto a, bool) { seg[a]--; });
					} else assert(false);
				} else {
					int q = cnd - N;
					auto [x, y] = queries[q];
					auto pt = layout.get_point(G[y].st);
					pt.for_each([&](auto a) { if (seg[a]) ans[q] = true; });
				}
			}
			evts.clear();
		}
	}

	for (int iter = 0; iter < 2; iter++) {
		std::swap(F, G);
		for (auto& it : queries) std::swap(it[0], it[1]);

		// Do the tree/cycle case
		
		std::vector<std::vector<std::map<int, int>>> by_root(N);
		for (int i = 0; i < N; i++) {
			if (F[i].root == i) {
				by_root[i].resize(F[i].cyc_len);
			}
		}

		for (int i = 0; i < N; i++) {
			if (F[i].is_cycle) {
				int r = F[i].root;
				int d = (G[i].depth - F[i].depth + F[i].cyc_len) % F[i].cyc_len;
				by_root[r][d][G[i].st] ++;
				by_root[r][d][G[i].en] --;
			}
		}

		for (int i = 0; i < N; i++) {
			if (F[i].root == i) {
				for (auto& mp : by_root[i]) {
					int cur = 0;
					for (auto& [_, v] : mp) {
						cur += v;
						v = cur;
					}
				}
			}
		}


		for (int q = 0; q < Q; q++) {
			auto [x, y] = queries[q];
			if (F[x].depth < G[y].depth) {

				int ny = y;
				for (int z = 0; (1 << z) <= F[x].depth; z++) {
					if (F[x].depth & (1 << z)) {
						ny = G[ny].jump[z];
					}
				}

				assert(G[ny].depth == G[y].depth - F[x].depth);

				int r = F[x].root;
				int d = G[ny].depth % F[x].cyc_len;
				assert(d >= 0);
				auto it = by_root[r][d].upper_bound(G[ny].st);
				if (it != by_root[r][d].begin() && std::prev(it)->second > 0) {
					ans[q] = true;
				}
			}
		}
	}

	for (bool r : ans) {
		cout << (r ? "YES" : "NO") << '\n';
	}

	return 0;
}
