#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int dfs(int j, G& g, F& f) {
	int low = val[j] = ++Time, x; z.push_back(j);
	for (auto e : g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,g,f));

	if (low == val[j]) {
		do {
			x = z.back(); z.pop_back();
			comp[x] = ncomps;
			cont.push_back(x);
		} while (x != j);
		f(cont); cont.clear();
		ncomps++;
	}
	return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
	int n = sz(g);
	val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	rep(i,0,n) if (comp[i] < 0) dfs(i, g, f);
}

#undef rep
#undef all
#undef sz
}

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

	int Z; cin >> Z;
	while (Z--) {
		int N, Q; cin >> N >> Q;

		int LO = N, HI = 2*N;

		N *= 3;

		int V = N + (N-1) + (N-1) + Q;
		std::vector<std::vector<int>> adj(V);
		std::vector<std::vector<int>> radj(V);
		auto add_edge = [&](int u, int v) -> void {
			adj[u].push_back(v);
			radj[v].push_back(u);
		};

		seg_tree::in_order_layout layout(N);

		auto up_seg_to_vert = [&](seg_tree::point a) -> int {
			if (a >= N) return layout.get_leaf_index(a);
			else return N + int(a)-1;
		};
		auto down_seg_to_vert = [&](seg_tree::point a) -> int {
			if (a >= N) return layout.get_leaf_index(a);
			else return N + (N-1) + int(a)-1;
		};

		for (seg_tree::point a(1); a < N; a++) {
			add_edge(up_seg_to_vert(a.c(0)), up_seg_to_vert(a));
			add_edge(up_seg_to_vert(a.c(1)), up_seg_to_vert(a));
			add_edge(down_seg_to_vert(a), down_seg_to_vert(a.c(0)));
			add_edge(down_seg_to_vert(a), down_seg_to_vert(a.c(1)));
		}

		for (int q = 0; q < Q; q++) {
			int a, b, c, d; cin >> a >> b >> c >> d; a--, c--;
			int v = N + (N-1) + (N-1) + q;
			layout.get_range(c, d).for_each([&](auto p) -> void {
				add_edge(up_seg_to_vert(p), v);
			});
			layout.get_range(a, b).for_each([&](auto p) -> void {
				add_edge(v, down_seg_to_vert(p));
			});
		}

		std::vector<int> res; res.reserve(N);
		bool done = false;
		std::string ans; ans.reserve(N);
		kactl::scc(adj, [&](std::vector<int>& a) -> void {
			if (done) return;

			{
				int l = 0;
				for (int i = 0; i < int(a.size()); i++) {
					if (a[i] < N) a[l++] = a[i];
				}
				a.resize(l);
			}

			assert(int(res.size()) < LO);
			if (int(res.size()) + int(a.size()) < LO) {
				res.insert(res.end(), a.begin(), a.end());
				return;
			} else if (int(res.size()) + int(a.size()) <= HI) {
				res.insert(res.end(), a.begin(), a.end());

				ans.assign(N, 'F');
				for (int i : res) ans[i] = 'R';

				done = true;
			} else {
				assert(int(a.size()) > N/3);

				done = true;
				// let's look for things
				{
					ans.assign(N, 'F');
					int num_set = 0;
					std::vector<bool> vis(V, false);
					std::vector<int> q; q.reserve(V);
					for (int v : a) {
						vis[v] = true;
						q.push_back(v);
						assert(v < N);
						ans[v] = 'R';
						num_set++;
					}
					for (int z = 0; z < int(q.size()); z++) {
						int cur = q[z];
						for (int nxt : adj[cur]) {
							if (vis[nxt]) continue;
							vis[nxt] = true;
							q.push_back(nxt);
							if (nxt < N) {
								ans[nxt] = 'R';
								num_set++;
							}
						}
					}
					assert(num_set >= LO);
					if (num_set <= HI) {
						return;
					}
				}
				{
					ans.assign(N, 'R');
					int num_set = 0;
					std::vector<bool> vis(V, false);
					std::vector<int> q; q.reserve(V);
					for (int v : a) {
						vis[v] = true;
						q.push_back(v);
						assert(v < N);
						ans[v] = 'F';
						num_set++;
					}
					for (int z = 0; z < int(q.size()); z++) {
						int cur = q[z];
						for (int nxt : radj[cur]) {
							if (vis[nxt]) continue;
							vis[nxt] = true;
							q.push_back(nxt);
							if (nxt < N) {
								ans[nxt] = 'F';
								num_set++;
							}
						}
					}
					assert(num_set >= LO);
					if (num_set <= HI) {
						return;
					}
				}
				ans.clear();
			}
		});
		if (ans.empty()) {
			cout << "NIE" << '\n';
		} else {
			cout << "TAK" << '\n';
			cout << ans << '\n';
		}
	}

	return 0;
}
