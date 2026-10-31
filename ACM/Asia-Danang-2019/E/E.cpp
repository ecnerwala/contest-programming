#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

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

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;

		vector<pair<int, int>> V(N);
		for (int i = 0; i < N; i++) {
			cin >> V[i].first;
			V[i].second = i;
		}
		std::sort(V.begin(), V.end());

		vector<int> A(N);
		for (int i = 0; i < N; i++) A[V[i].second] = i;

		struct query_t {
			int l, r;
			int u, v;
			int idx;
		};
		vector<query_t> queries(Q);

		struct bsearch_query_t {
			int l, r;
			int u;
			int* res;
		};
		vector<bsearch_query_t> bsearch_queries; bsearch_queries.reserve(2*Q);

		for (int q = 0; q < Q; q++) {
			int l, r, u, v; cin >> l >> r >> u >> v; l--, u--;
			queries[q].l = l;
			queries[q].r = r;
			queries[q].idx = q;
			bsearch_queries.push_back({l, r, u, &queries[q].u});
			if (v < r-l) {
				bsearch_queries.push_back({l, r, v, &queries[q].v});
			} else {
				queries[q].v = N;
			}
		}

		std::y_combinator([&](auto self, int mi, int ma, std::vector<int> curA, std::vector<bsearch_query_t> curQ) -> void {
			if (ma - mi == 1) {
				for (auto& q : curQ) *q.res = mi;
				return;
			}
			int md = mi + (ma - mi) / 2;

			std::vector<int> loA, hiA;
			std::vector<bsearch_query_t> loQ, hiQ;

			{
				std::vector<int> pref(curA.size()+1);
				for (int i = 0; i < int(curA.size()); i++) pref[i+1] = pref[i] + (curA[i] < md);

				loA.reserve(pref.back());
				hiA.reserve(int(curA.size()) - pref.back());
				for (int i = 0; i < int(curA.size()); i++) (curA[i] < md ? loA : hiA).push_back(curA[i]);
				curA = {};

				loQ.reserve(curQ.size());
				hiQ.reserve(curQ.size());

				for (auto q : curQ) {
					int pl = pref[q.l];
					int pr = pref[q.r];
					int v = pr - pl;
					if (v <= q.u) {
						q.u -= v;
						q.l -= pl;
						q.r -= pr;
						hiQ.push_back(q);
					} else {
						q.l = pl;
						q.r = pr;
						loQ.push_back(q);
					}
				}

				curQ = {};
				loQ.shrink_to_fit();
				hiQ.shrink_to_fit();
			}

			self(mi, md, std::move(loA), std::move(loQ));
			self(md, ma, std::move(hiA), std::move(hiQ));
		})(0, N, A, std::move(bsearch_queries));

		seg_tree::circular_layout layout(N);
		struct seg_node {
			std::array<std::array<int, 3>, 30> v;
		};
		std::vector<seg_node> seg(2*N);
		for (auto& n : seg) for (auto& r : n.v) r.fill(-1);

		std::vector<int64_t> ans(Q, -1);
		std::sort(queries.begin(), queries.end(), [&](const auto& a, const auto& b) { return a.r < b.r; });

		int cur_r = 0;
		for (auto q : queries) {
			for (; cur_r < q.r; cur_r++) {
				int v = A[cur_r];
				int m = V[v].first;
				assert(V[v].second == cur_r);

				array<int, 3> p;
				p[0] = cur_r;
				p[1] = v >= 1 && V[v-1].first == m ? V[v-1].second : -1;
				p[2] = v >= 2 && V[v-2].first == m ? V[v-2].second : -1;

				layout.get_point(v).for_each([&](seg_tree::point a) {
					auto& n = seg[a];
					for (int b = 0; b < 30; b++) {
						if (m & (1 << b)) {
							for (int c = 0; c < 3; c++) {
								setmax(n.v[b][c], p[c]);
							}
						}
					}
				});
			}

			array<int, 3> f{0,0,0};
			if (q.v - q.u == 0) {
			} else if (q.v - q.u == 1) {
				f[0] = V[q.u].first;
			} else {
				f[0] = V[q.u].first;
				if (q.l <= V[q.u+1].second && V[q.u+1].second < q.r) {
					f[0] |= V[q.u+1].first;
					f[1] = V[q.u].first == V[q.u+1].first ? V[q.u].first : 0;
				}
				layout.get_range(q.u + 2, q.v).for_each([&](seg_tree::point a, bool) {
					const auto& n = seg[a];
					for (int b = 0; b < 30; b++) {
						for (int c = 0; c < 3; c++) {
							f[c] |= ((n.v[b][c] >= q.l) << b);
						}
					}
				});
			}

			ans[q.idx] = int64_t(f[0]) + int64_t(f[1]) + int64_t(f[2]);
		}

		for (auto v : ans) {
			cout << v << '\n';
		}
	}

	return 0;
}
