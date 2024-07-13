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

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#undef sz
#undef all
#undef rep

/**
 * Author: Simon Lindholm
 * Date: 2017-04-20
 * License: CC0
 * Source: own work
 * Description: Container where you can add lines of the form kx+m, and query maximum values at points x.
 *  Useful for dynamic programming (``convex hull trick'').
 * Time: O(\log N)
 * Status: stress-tested
 */

struct Line {
	mutable ll k, m, p;
	bool operator<(const Line& o) const { return k < o.k; }
	bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	static const ll inf = LLONG_MAX;
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) return x->p = inf, 0;
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ll query(ll x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};
}

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

template <typename F> struct reverse_comparator_t {
	F f;
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) & {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const& {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) && {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const&& {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename F> constexpr reverse_comparator_t<std::decay_t<F>> reverse_comparator(F&& f) {
	return { std::forward<F>(f) };
}

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
	};
	std::vector<Node> nodes;
	int root = -1;

	CartesianTree() {}

	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

private:
	CartesianTree(std::vector<Node>&& nodes_, int root_) : nodes(std::move(nodes_)), root(root_) {}

public:

	// min-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_min_tree(const std::vector<T>& v, Comp comp = Comp()) {
		std::vector<Node> nodes(v.size()*2+1);
		std::vector<int> stk; stk.reserve(v.size());
		int root = -1;
		for (int i = 0; i <= int(v.size()); i++) {
			int cur = 2*i;
			nodes[cur].l = i;
			nodes[cur].r = i-1;
			nodes[cur].m = i-1;
			nodes[cur].c = {-1, -1};
			while (!stk.empty() && (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
				int nxt = stk.back(); stk.pop_back();
				nodes[nxt].c[1] = cur;
				nodes[nxt].r = nodes[cur].r;
				cur = nxt;
			}
			if (i == int(v.size())) {
				root = cur;
				break;
			}
			nodes[2*i+1].l = nodes[cur].l;
			nodes[2*i+1].m = i;
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> C(N);
	for (auto& c : C) cin >> c;
	std::vector<int> A(N+1);
	for (int i = 1; i <= N-1; i++) cin >> A[i];

	seg_tree::in_order_layout layout(N+1);
	struct lichao_line {
		int64_t k; // coeff
		int64_t m; // const
	};
	auto eval_value = [&](lichao_line a, int val) -> int64_t {
		assert(1 <= val && val <= N);
		return a.k * val + a.m;
	};
	auto eval_node = [&](lichao_line a, seg_tree::point p) -> int64_t {
		int val = layout.get_node_split(p);
		return eval_value(a, val);
	};

	// lichao tree is a bst
	const int64_t INF = 1e18;
	std::vector<lichao_line> seg(layout.N, lichao_line{0, INF});

	auto update_lichao = [&](int k, int64_t m) -> std::tuple<seg_tree::point, int, lichao_line> {
		cerr << "update lichao " << k << ' ' << m << '\n';
		lichao_line nxt{k, m};
		seg_tree::point a(1);
		int msk = 0;
		while (a < layout.N) {
			msk <<= 1;
			if (eval_node(nxt, a) < eval_node(seg[a], a)) {
				msk |= 1;
				std::swap(nxt, seg[a]);
			}
			if (nxt.k < seg[a].k) {
				// better when coeff is smaller
				a = a.c(1);
			} else {
				a = a.c(0);
			}
		}
		cerr << "  "; for (auto line : seg) { cerr << line.k << "x+" << line.m << ' '; } cerr << '\n';
		return {a, msk, nxt};
	};
	auto rollback_lichao = [&](std::tuple<seg_tree::point, int, lichao_line> r) -> void {
		auto [a, msk, nxt] = r;
		while (a > 1) {
			a = a.p();
			if (msk & 1) {
				std::swap(nxt, seg[a]);
			}
			msk >>= 1;
		}
		cerr << "undo update lichao " << nxt.k << ' ' << nxt.m << '\n';
		cerr << "  "; for (auto line : seg) { cerr << line.k << "x+" << line.m << ' '; } cerr << '\n';
	};
	auto query_lichao = [&](int v) -> int64_t {
		assert(1 <= v && v <= N);
		int64_t res = eval_value(seg[1], v);
		layout.get_point(v).for_parents_up([&](seg_tree::point a) -> void {
			res = std::min(res, eval_value(seg[a], v));
		});
		return res;
	};

	auto ct = CartesianTree::build_max_tree(C);

	std::vector<kactl::LineContainer> containers(2*N+1);
	std::y_combinator([&](auto self, int cur) -> kactl::LineContainer {
		if (cur % 2 == 0) {
			// We're a leaf: query the lichao tree
			int64_t v;
			int idx = cur/2;
			if (idx == 0) {
				v = 0;
			} else {
				v = query_lichao(idx);
			}
			cerr << "at " << idx << " " << "value " << v << " + " << A[idx] << " = " << v + A[idx] << '\n';
			v += A[idx];
			if (idx == N) {
				cout << v << '\n';
				exit(0);
			}
			kactl::LineContainer res;
			// we're trying to add -(cur/2) * MX + v, but we negate it to get mins
			res.add(idx, -v);
			return res;
		} else {
			kactl::LineContainer l = self(ct.nodes[cur].c[0]);
			// Now find own value and add in the next one
			int c = C[cur/2];
			int64_t v = -l.query(c);
			cerr << "at " << cur/2 << "-" << cur/2+1 << " " << "height " << c << " " << v << '\n';
			for (auto line : l) {
				cerr << "  " << line.k << ' ' << line.m << '\n';
			}
			// now update the lichao tree with v, c
			auto to_rollback = update_lichao(c, v);
			kactl::LineContainer r = self(ct.nodes[cur].c[1]);
			rollback_lichao(to_rollback);
			if (r.size() > l.size()) std::swap(l, r);
			for (auto line : r) {
				l.add(line.k, line.m);
			}
			return l;
		}
	})(ct.root);

	return 0;
}
