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

struct treap_node {
	std::array<treap_node*, 2> c = {nullptr, nullptr};
	std::mt19937_64::result_type pri;
	int k;
	int t;
	std::array<int, 2> cnt;
	void update() {
		cnt = {0, 0};
		cnt[t]++;
		if (c[0]) cnt[0] += c[0]->cnt[0], cnt[1] += c[0]->cnt[1];
		if (c[1]) cnt[0] += c[1]->cnt[0], cnt[1] += c[1]->cnt[1];
	}
};

int get_max(treap_node* t) {
	std::array<int, 2> cnt{0, 0};
	while (t) {
		int l = cnt[0] + (t->c[0] ? t->c[0]->cnt[0] : 0);
		int r = cnt[1] + (t->c[1] ? t->c[1]->cnt[1] : 0);
		int d;
		if (l > r) {
			// go left
			d = 0;
		} else if (l < r) {
			d = 1;
		} else {
			d = !t->t;
		}
		if (d == 0) {
			cnt[1] = r + (t->t == 1);
			t = t->c[0];
		} else if (d == 1) {
			cnt[0] = l + (t->t == 0);
			t = t->c[1];
		} else assert(false);
	}
	return std::min(cnt[0], cnt[1]);
}

std::pair<treap_node*, treap_node*> split_at(treap_node* a, int k) {
	if (!a) return {nullptr, nullptr};
	treap_node *l, *r;
	if (a->k < k) {
		l = a;
		std::tie(l->c[1], r) = split_at(a->c[1], k);
	} else {
		r = a;
		std::tie(l, r->c[0]) = split_at(a->c[0], k);
	}
	a->update();
	return {l, r};
}

void insert(treap_node* &r, treap_node* n) {
	assert(!n->c[0] && !n->c[1]);
	if (!r || n->pri < r->pri) {
		std::tie(n->c[0], n->c[1]) = split_at(r, n->k);
		r = n;
	} else {
		if (n->k < r->k) {
			insert(r->c[0], n);
		} else {
			insert(r->c[1], n);
		}
	}
	r->update();
}

treap_node* merge(treap_node *l, treap_node *r) {
	if (!l) return r;
	if (!r) return l;
	treap_node *n;
	if (l->pri < r->pri) {
		n = l;
		n->c[1] = merge(l->c[1], r);
	} else {
		n = r;
		n->c[0] = merge(l, r->c[0]);
	}
	n->update();
	return n;
}

void remove(treap_node* &r, treap_node* n) {
	assert(r);
	if (r == n) {
		r = merge(n->c[0], n->c[1]);
		n->c[0] = n->c[1] = nullptr;
		n->update();
	} else {
		if (n->k < r->k) {
			remove(r->c[0], n);
		} else {
			remove(r->c[1], n);
		}
		r->update();
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::mt19937_64 mt(48);
	std::vector<treap_node> nodes(2*N);
	for (int i = 0; i < N; i++) {
		for (int z = 0; z < 2; z++) {
			treap_node* n = &nodes[2*i+z];
			n->c = {nullptr, nullptr};
			n->pri = mt();
			n->k = i;
			n->t = z;
			n->update();
		}
	}

	std::vector<treap_node*> roots(M);
	std::vector<int> cnts(M);
	for (int i = 0; i < N; i++) {
		cnts[A[i]]++;
		insert(roots[A[i]], &nodes[2*i+1]);
		insert(roots[(A[i]+1)%M], &nodes[2*i+0]);
	}

	seg_tree::in_order_layout layout(M);
	std::vector<std::array<std::array<int, 2>, 2>> seg(2*M);

	auto update_node = [&](seg_tree::point a) {
		assert(a < layout.N);
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				seg[a][i][j] = std::min(
					seg[a.c(0)][i][0] + seg[a.c(1)][0][j],
					seg[a.c(0)][i][1] + seg[a.c(1)][1][j]
				);
			}
		}
	};

	for (int v = 0; v < M; v++) {
		seg_tree::point a = layout.get_point(v);
		seg[a][0][0] = get_max(roots[v]);
		seg[a][1][0] = 0;
		seg[a][1][1] = N;
		seg[a][0][1] = cnts[v];
	}
	for (seg_tree::point a(M-1); a > 0; a--) update_node(a);

	for (int q = 0; q < Q; q++) {
		int i, v; cin >> i >> v; i--;
		{
			cnts[A[i]]--;
			remove(roots[A[i]], &nodes[2*i+1]);
			remove(roots[(A[i]+1)%M], &nodes[2*i+0]);
			seg_tree::point a = layout.get_point(A[i]);
			seg_tree::point b = layout.get_point((A[i]+1)%M);
			seg[a][0][1] = cnts[A[i]];
			seg[a][0][0] = get_max(roots[A[i]]);
			seg[b][0][0] = get_max(roots[(A[i]+1)%M]);
			a.for_parents_up(update_node);
			b.for_parents_up(update_node);
		}

		A[i] = v;

		{
			cnts[A[i]]++;
			insert(roots[A[i]], &nodes[2*i+1]);
			insert(roots[(A[i]+1)%M], &nodes[2*i+0]);
			seg_tree::point a = layout.get_point(A[i]);
			seg_tree::point b = layout.get_point((A[i]+1)%M);
			seg[a][0][1] = cnts[A[i]];
			seg[a][0][0] = get_max(roots[A[i]]);
			seg[b][0][0] = get_max(roots[(A[i]+1)%M]);
			a.for_parents_up(update_node);
			b.for_parents_up(update_node);
		}

		cout << std::min({seg[1][0][0], seg[1][1][1], N / 2}) << '\n';
	}

	return 0;
}
