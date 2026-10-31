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

	int N, Q; cin >> N >> Q;

	struct seg_node {
		std::array<seg_node*, 2> c{nullptr, nullptr};
	};

	seg_tree::in_order_layout layout(N);

	auto concat_seg = [&](this auto&& self, seg_node* x, seg_node* y, seg_tree::point a = seg_tree::point(1)) -> seg_node* {
		if (!x) return y;
		if (!y) return x;
		assert(a < layout.N);
		assert(!x->c[1] || !y->c[0]);
		return new seg_node{{self(x->c[0], y->c[0], a.c(0)), self(x->c[1], y->c[1], a.c(1))}};
	};

	std::vector<bool> alive(Q);

	auto build_seg = [&](this auto&& self, int q, seg_tree::point a = seg_tree::point(1)) -> seg_node* {
		auto s = new seg_node{};
		if (a < layout.N) {
			int d = q >= layout.get_node_split(a);
			s->c[d] = self(q, a.c(d));
		}
		return s;
	};

	auto query_seg_leftmost = [&](this auto&& self, seg_node* &s, seg_tree::point a = seg_tree::point(1)) -> std::optional<int> {
		if (!s) return std::nullopt;
		if (a >= layout.N) {
			int i = layout.get_leaf_index(a);
			if (alive[i]) {
				return i;
			}
		} else {
			for (int z = 0; z < 2; z++) {
				auto r = self(s->c[z], a.c(z));
				if (r) return r;
			}
		}
		s = nullptr;
		return std::nullopt;
	};

	std::mt19937_64 mt(48);
	struct treap_node {
		std::array<treap_node*, 2> c = {nullptr, nullptr};
		std::mt19937_64::result_type pri;
		seg_node* own = nullptr;
		seg_node* lazy = nullptr;
		int sz = 1;
		bool lazy_flip = false;
	};
	auto flip_treap = [&](treap_node* t) {
		assert(t);
		t->lazy_flip ^= 1;
		std::swap(t->c[0], t->c[1]);
	};
	auto update_treap = [&](treap_node* t) {
		assert(t);
		t->sz = (t->c[0] ? t->c[0]->sz : 0) + 1 + (t->c[1] ? t->c[1]->sz : 0);
	};
	auto downdate_treap_flip = [&](treap_node* t) {
		assert(t);
		if (t->lazy_flip) {
			if (t->c[0]) flip_treap(t->c[0]);
			if (t->c[1]) flip_treap(t->c[1]);
			t->lazy_flip = false;
		}
	};
	auto downdate_treap = [&](treap_node* t) {
		assert(t);
		downdate_treap_flip(t);
		if (t->lazy) {
			for (auto ch : t->c) {
				if (ch) {
					ch->lazy = concat_seg(ch->lazy, t->lazy);
				}
			}
			t->own = concat_seg(t->own, t->lazy);
			t->lazy = nullptr;
		}
	};

	auto split_treap = [&](this auto&& self, treap_node* r, int l) -> std::pair<treap_node*, treap_node*> {
		if (l == 0) return {nullptr, r};
		if (l == r->sz) return {r, nullptr};

		downdate_treap(r);

		treap_node *a, *b;
		int l_sz = (r->c[0] ? r->c[0]->sz : 0);
		if (l <= l_sz) {
			b = r;
			std::tie(a, b->c[0]) = self(r->c[0], l);
		} else {
			a = r;
			std::tie(a->c[1], b) = self(r->c[1], l - 1 - l_sz);
		}

		update_treap(r);
		return {a, b};
	};

	auto concat_treap = [&](this auto&& self, treap_node* a, treap_node* b) -> treap_node* {
		if (!a) return b;
		if (!b) return a;

		treap_node *r;
		if (a->pri < b->pri) {
			downdate_treap(a);
			r = a;
			r->c[1] = self(a->c[1], b);
		} else {
			downdate_treap(b);
			r = b;
			r->c[0] = self(a, b->c[0]);
		}
		update_treap(r);
		return r;
	};

	std::vector<treap_node> nodes(N);
	treap_node* root;
	for (int i = 0; i < N; i++) {
		auto n = &nodes[i];
		n->pri = mt();
		update_treap(n);
		root = concat_treap(root, n);
	}

	auto do_insert = [&](int r, int q) -> void {
		//cerr << "do_insert " << r << ' ' << q << '\n';
		assert(1 <= r && r <= N);
		assert(0 <= q && q < Q);
		alive[q] = true;
		auto [a, b] = split_treap(root, r);
		a->lazy = concat_seg(a->lazy, build_seg(q));
		root = concat_treap(a, b);
	};

	auto do_reverse = [&](int r) -> void {
		//cerr << "do_reverse " << r << '\n';
		assert(1 <= r && r <= N);
		auto [a, b] = split_treap(root, r);
		flip_treap(a);
		root = concat_treap(a, b);
	};

	auto do_delete = [&](int q) -> void {
		//cerr << "do_delete " << q << '\n';
		assert(0 <= q && q < Q);
		alive[q] = false;
	};

	auto do_query = [&](int p) -> std::optional<int> {
		//cerr << "do_query " << p << '\n';
		assert(0 <= p && p < N);
		auto ans = [&](this auto&& self, treap_node* cur, int l) -> std::optional<int> {
			assert(cur);
			assert(0 <= l && l < cur->sz);
			int l_sz = (cur->c[0] ? cur->c[0]->sz : 0);
			if (l != l_sz) {
				downdate_treap_flip(cur);
				auto r = self(cur->c[l > l_sz], l > l_sz ? l - l_sz - 1 : l);
				if (r) return r;
			} else {
				auto r = query_seg_leftmost(cur->own);
				if (r) return r;
			}
			return query_seg_leftmost(cur->lazy);
		}(root, p);
		//cerr << ans.value_or(-1) << '\n';
		return ans;
	};

	int prv_ans = 0;
	for (int q = 0; q < Q; q++) {
		int a, b, c; cin >> a >> b >> c;
		if (a == 1) {
			assert(1 <= b && b <= N);
			int r = (b + prv_ans - 1) % N + 1;
			do_insert(r, q);
		} else if (a == 2) {
			assert(1 <= b && b <= N);
			int r = (b + prv_ans - 1) % N + 1;
			do_reverse(r);
		} else if (a == 3) {
			assert(1 <= b && b <= Q);
			int x = (b + prv_ans - 1) % Q + 1;
			do_delete(x-1);
		} else assert(false);

		assert(1 <= c && c <= N);
		int p = (c + prv_ans - 1) % N + 1;
		prv_ans = do_query(p-1).value_or(-1) + 1;
		cout << prv_ans << '\n';
	}

	return 0;
}
