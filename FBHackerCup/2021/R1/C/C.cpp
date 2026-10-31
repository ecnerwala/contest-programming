#include<bits/stdc++.h>

/**
 * Top tree!
 *
 * Usage:
 *   Make a `struct T : public top_tree_node_base<T>` (CRTP), which implements
 *     void update()
 *     void downdate()
 *     void do_flip_path()
 *     void do_other_operation() ...
 *   When update() is called, you can assume downdate() has already been called.
 *
 *   In general, do_op() should eagerly apply the operation but not touch the
 *   children. In downdate(), you can push down to the children with ch->do_op().
 *   WARNING: if different operations do not trivially commute, you *must*
 *   implement a way to swap/alter them to compose in a consistent order, and you
 *   must use that order when implementing downdate(). This can be nontrivial!
 *
 *   Creating vertices:
 *     n->is_path = n->is_vert = true;
 *     n->update();
 *
 *   Creating edges: no setup/update() needed, just call
 *     link(e, va, vb);
 *
 *   Updates:
 *     auto cur = get_path(va, vb); // or get_subtree(va, vb)
 *     cur->do_stuff();
 *     cur->downdate();
 *     cur->update_all();
 *
 * Node types:
 *   path edges: compress(c[0], self, c[1])
 *     assert(is_path && !is_vert);
 *     assert(c[0] && c[1]);
 *     assert(c[0]->is_path && c[1]->is_path);
 *     assert(!c[2]);
 *   (path) vertices: self + rake(c[0], c[1])
 *     assert(is_path && is_vert);
 *     assert(!c[2]);
 *     if (c[0]) assert(!c[0]->is_path);
 *     if (c[1]) assert(!c[1]->is_path);
 *   non-path edges: rake(c[0], self + c[2], c[1])
 *     assert(!is_path && !is_vert);
 *     assert(c[2])
 *     assert(c[2]->is_path);
 *     if (c[0]) assert(!c[0]->is_path);
 *     if (c[1]) assert(!c[1]->is_path);
 */

template <typename top_tree_node> struct top_tree_node_base {
private:
	top_tree_node* derived_this() {
		return static_cast<top_tree_node*>(this);
	}
	const top_tree_node* derived_this() const {
		return static_cast<const top_tree_node*>(this);
	}
public:
	mutable top_tree_node* p = nullptr;
	std::array<top_tree_node*, 3> c{nullptr, nullptr, nullptr};

	int d() const {
		assert(p);
		if (this == p->c[0]) {
			return 0;
		} else if (this == p->c[1]) {
			return 1;
		} else if (this == p->c[2]) {
			return 2;
		} else assert(false);
	}
	top_tree_node*& p_c() const { return p->c[d()]; } // p->c which points to you

	// 3 types of verts: path edges, path verts, non-path edges
	bool is_path;
	bool is_vert;

	bool r() const { return !p || p->is_path != is_path; }

private:
	// Convenience wrappers for the derived functions.
	void do_flip_path() {
		derived_this()->do_flip_path();
	}
	void downdate() {
		derived_this()->downdate();
	}
	void update() {
		derived_this()->update();
	}

public:
	void downdate_all() {
		if (p) p->downdate_all();
		downdate();
	}

	// Returns the root
	top_tree_node* update_all() {
		top_tree_node* cur = derived_this();
		cur->update();
		while (cur->p) {
			cur = cur->p;
			cur->update();
		}
		return cur;
	}

	void downdate_tree() {
		downdate();
		for (top_tree_node* n : c) if (n) n->downdate_tree();
	}

private:

	void rot() {
		assert(!is_vert);
		assert(!r());
		top_tree_node* pa = p;
		int x = d(); assert(x == 0 || x == 1);
		top_tree_node* ch = c[!x];

		if (pa->p) pa->p_c() = derived_this();
		this->p = pa->p;

		pa->c[x] = ch;
		if (ch) ch->p = pa;

		this->c[!x] = pa;
		pa->p = derived_this();

		pa->update();
	}

	void rot_2(int c_d) {
		assert(!is_vert);
		assert(!r());
		assert(c[c_d]);
		assert(!c[c_d]->is_vert);

		if (d() == c_d) {
			rot();
			return;
		}

		top_tree_node* pa = p;
		int x = d(); assert(x == 0 || x == 1);
		assert(c_d == !x);
		top_tree_node* ch = c[c_d]->c[!x];

		if (pa->p) pa->p_c() = derived_this();
		this->p = pa->p;

		pa->c[x] = ch;
		if (ch) ch->p = pa;

		this->c[c_d]->c[!x] = pa;
		pa->p = this->c[c_d];

		pa->update();
	}

	void splay_dir(int x) {
		while (!r() && d() == x) {
			if (!p->r() && p->d() == x) {
				p->rot();
			}
			rot();
		}
	}

	void splay_2(int c_d) {
		assert(!is_vert && is_path);
		assert(c[c_d] && !c[c_d]->is_vert);
		while (!r()) {
			if (!p->r()) {
				if (p->d() == d()) {
					p->rot();
				} else {
					rot_2(c_d);
				}
			}
			rot_2(c_d);
		}
	}

	void splay_2() {
		assert(!is_vert && is_path);
		assert(!r());
		p->splay_2(d());
	}

	void splay_vert() {
		assert(is_vert);
		if (r()) {
			return;
		}
		p->splay_dir(d());
		if (p->r()) {
			return;
		}

		assert(p->d() != d());
		// we have a preference to be the left child
		if (d() == 1) {
			p->rot();
		}
		assert(d() == 0);

		p->splay_2();
		assert(d() == 0);
		assert(p->d() == 1);
		assert(p->p->r());
	}

	void splay() {
		assert(!is_vert);
		while (!r()) {
			if (!p->r()) {
				if (p->d() == d()) {
					p->rot();
				} else {
					rot();
				}
			}
			rot();
		}
	}

	top_tree_node* cut_right() {
		assert(is_vert && is_path);
		splay_vert();

		if (r() || d() == 1) {
			assert(r() || (d() == 1 && p->r()));
			assert(c[0] == nullptr);
			return nullptr;
		}

		top_tree_node* pa = p;
		assert(pa->r() || (pa->d() == 1 && pa->p->r()));
		assert(!pa->is_vert);
		assert(pa->is_path);
		assert(pa->c[0] == this);
		assert(pa->c[2] == nullptr);

		if (pa->p) pa->p_c() = derived_this();
		this->p = pa->p;

		pa->is_path = false;
		pa->c[2] = pa->c[1]; // don't need to change the parent

		pa->c[0] = c[0]; if (c[0]) c[0]->p = pa;
		pa->c[1] = c[1]; if (c[1]) c[1]->p = pa;

		c[0] = nullptr;
		c[1] = pa; pa->p = derived_this();
		assert(c[2] == nullptr);

		assert(c[0] == nullptr);

		pa->update();
		return pa;
	}

	top_tree_node* splice_non_path() {
		assert(!is_path);
		assert(!is_vert);

		splay();
		assert(p && p->is_vert && p->is_path);
		p->cut_right();

		if (!p->is_path) rot();
		assert(p && p->is_vert && p->is_path);
		assert(p->r() || (p->d() == 1 && p->p->r()));
		assert(p->c[d()] == this && p->c[!d()] == nullptr);

		top_tree_node* pa = p;

		if (pa->p) pa->p_c() = derived_this();
		this->p = pa->p;

		pa->c[0] = c[0]; if (c[0]) c[0]->p = pa;
		pa->c[1] = c[1]; if (c[1]) c[1]->p = pa;

		assert(c[2] && c[2]->is_path);
		c[1] = c[2]; // don't need to change parent
		c[0] = pa; pa->p = derived_this();
		c[2] = nullptr;

		is_path = true;

		pa->update();
		return pa;
	}

	// Return the topmost vertex which was spliced into
	top_tree_node* splice_all() {
		top_tree_node* res = nullptr;
		for (top_tree_node* cur = derived_this(); cur; cur = cur->p) {
			if (!cur->is_path) {
				res = cur->splice_non_path();
			}
			assert(cur->is_path);
		}
		return res;
	}

public:
	// Return the topmost vertex which was spliced into
	top_tree_node* expose() {
		assert(is_vert);
		downdate_all();

		top_tree_node* res = splice_all();

		cut_right();

		update_all();

		return res;
	}

	// Return the topmost vertex which was spliced into
	top_tree_node* expose_edge() {
		assert(!is_vert);
		downdate_all();

		top_tree_node* v = is_path ? c[1] : c[2];
		v->downdate();

		while (!v->is_vert) {
			v = v->c[0];
			v->downdate();
		}

		top_tree_node* res = v->splice_all();
		v->cut_right();
		v->update_all();

		assert(!p);
		assert(v == c[1]);

		return res;
	}

	// Return the new root
	top_tree_node* meld_path_end() {
		assert(!p);
		top_tree_node* rt = derived_this();
		while (true) {
			rt->downdate();
			if (rt->is_vert) break;
			rt = rt->c[1];
		}
		assert(rt->is_vert);
		rt->splay_vert();
		if (rt->c[0] && rt->c[1]) {
			top_tree_node* ch = rt->c[1];
			while (true) {
				ch->downdate();
				if (!ch->c[0]) break;
				ch = ch->c[0];
			}
			ch->splay();
			assert(ch->c[0] == nullptr);

			ch->c[0] = rt->c[0];
			ch->c[0]->p = ch;

			rt->c[0] = nullptr;

			ch->update();
		} else if (rt->c[0]) {
			rt->c[1] = rt->c[0];
			rt->c[0] = nullptr;
		}
		assert(rt->c[0] == nullptr);
		return rt->update_all();
	}

	void make_root() {
		expose();

		top_tree_node* rt = derived_this();
		while (rt->p) {
			assert(rt->d() == 1);
			rt = rt->p;
		}
		rt->do_flip_path();
		rt->meld_path_end();

		expose();

		assert(!p);
	}

	// link v2 as a child of v1 with edge e
	friend void link(top_tree_node* e, top_tree_node* v1, top_tree_node* v2) {
		assert(e && v1 && v2);
		assert(!e->c[0] && !e->c[1] && !e->c[2]);
		v1->expose(); while (v1->p) v1 = v1->p;
		v2->make_root();

		assert(!v1->p);
		assert(!v2->p);

		e->is_path = true, e->is_vert = false;
		e->c[0] = v1;
		v1->p = e;
		e->c[1] = v2;
		v2->p = e;
		e->update();
	}

	// Cuts the edge e
	// Returns the top-tree-root of the two halves; they are not necessarily the split vertices.
	friend std::pair<top_tree_node*, top_tree_node*> cut(top_tree_node* e) {
		assert(!e->is_vert);
		e->expose_edge();

		assert(!e->p);
		assert(e->is_path);

		top_tree_node* l = e->c[0];
		top_tree_node* r = e->c[1];
		assert(l && r);

		e->c[0] = e->c[1] = nullptr;
		l->p = r->p = nullptr;

		assert(e->c[2] == nullptr);

		l = l->meld_path_end();

		return {l, r};
	}

	friend top_tree_node* get_path(top_tree_node* a, top_tree_node* b) {
		assert(a->is_vert && b->is_vert);
		a->make_root();
		b->expose();
		if (a == b) {
			assert(!b->p);
			return b;
		}
		assert(!b->p->p);
		return b->p;
	}

	friend top_tree_node* get_subtree(top_tree_node* rt, top_tree_node* n) {
		rt->make_root();
		n->expose();
		return n;
	}
};

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

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

using num = modnum<int(1e9)+7>;
struct top_tree_node : public top_tree_node_base<top_tree_node> {
	// Apply lazy flip first, then lazy
	// This means if a flip comes down, we need to swap lazies
	bool flip_path = false;
	num sz = 0;
	num val = 0;
	num lazy_need_right = 0;
	num lazy_need_left = 0;
	num lazy_have = 0;

	void do_flip_path() {
		assert(is_path);
		if (!is_vert) {
			std::swap(lazy_need_right, lazy_need_left);
		}
		std::swap(c[0], c[1]);
		flip_path ^= 1;
	}

	void do_lazy(num need_right, num need_left, num have) {
		if (is_vert) {
			assert(is_path);
			need_right += need_left;
			need_left = 0;
			have = 0;
		} else if (is_path) {
			val += have;
			assert(c[0] && c[1]);
			val += need_left * c[0]->sz;
			val += need_right * c[1]->sz;
		} else {
			assert(need_left == 0);
			assert(have == 0);
			assert(c[2]);
			val += need_right * c[2]->sz;
		}

		lazy_need_right += need_right;
		lazy_need_left += need_left;
		lazy_have += have;
	}

	void downdate() {
		if (flip_path) {
			assert(is_path);
			if (!is_vert) {
				if (c[0]) c[0]->do_flip_path();
				if (c[1]) c[1]->do_flip_path();
			}
			flip_path = false;
		}
		// push down lazies
		if (is_vert) {
			assert(lazy_need_left == 0);
			assert(lazy_have == 0);
			if (c[0]) c[0]->do_lazy(lazy_need_right, lazy_need_left, lazy_have);
			if (c[1]) c[1]->do_lazy(lazy_need_right, lazy_need_left, lazy_have);
		} else if (is_path) {
			assert(c[0] && c[1]);
			c[0]->do_lazy(lazy_need_right, lazy_need_left, lazy_have + lazy_need_right * c[1]->sz);
			c[1]->do_lazy(lazy_need_right, lazy_need_left, lazy_have + lazy_need_left * c[0]->sz);
		} else {
			assert(lazy_need_left == 0);
			assert(lazy_have == 0);
			if (c[0]) c[0]->do_lazy(lazy_need_right, lazy_need_left, lazy_have);
			if (c[1]) c[1]->do_lazy(lazy_need_right, lazy_need_left, lazy_have);
			assert(c[2]);
			c[2]->do_lazy(lazy_need_right, lazy_need_left, lazy_have);
		}
		lazy_need_right = lazy_need_left = lazy_have = 0;
	}

	void update() {
		if (is_vert) {
			assert(is_path);
			sz = 1;
			if (c[0]) sz += c[0]->sz;
			if (c[1]) sz += c[1]->sz;
		} else if (is_path) {
			sz = 0;
			assert(c[0] && c[1]);
			sz += c[0]->sz;
			sz += c[1]->sz;
		} else {
			sz = 0;
			if (c[0]) sz += c[0]->sz;
			if (c[1]) sz += c[1]->sz;
			assert(c[2]);
			sz += c[2]->sz;
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N; cin >> N;
		std::vector<std::pair<int, std::array<int, 2>>> edges(N-1);
		for (auto& e : edges) {
			int u, v, w; cin >> u >> v >> w; u--, v--;
			e = {w, {u, v}};
		}
		std::sort(edges.begin(), edges.end());
		std::reverse(edges.begin(), edges.end());

		std::vector<top_tree_node> vert_nodes(N);
		for (int i = 0; i < N; i++) {
			top_tree_node* n = &vert_nodes[i];
			n->is_path = n->is_vert = true;
			n->update();
		}

		std::vector<top_tree_node> edge_nodes(N-1);
		num tot = 0;
		for (int e = 0; e < N-1; e++) {
			//cerr << edges[e].second[0] << ".." << edges[e].second[1] << ' ' << edges[e].first << '\n';
			num w = num(edges[e].first);
			auto [a, b] = edges[e].second;
			vert_nodes[a].make_root();
			vert_nodes[b].make_root();

			//cerr << a << ' ' << vert_nodes[a].sz << '\n';
			//cerr << b << ' ' << vert_nodes[b].sz << '\n';

			tot += w * vert_nodes[a].sz * vert_nodes[b].sz;
			edge_nodes[e].val = w * vert_nodes[a].sz * vert_nodes[b].sz;
			vert_nodes[a].do_lazy(w * vert_nodes[b].sz, 0, 0);
			vert_nodes[b].do_lazy(w * vert_nodes[a].sz, 0, 0);

			link(&edge_nodes[e], &vert_nodes[a], &vert_nodes[b]);
		}

		vert_nodes[0].make_root();
		vert_nodes[0].downdate_tree();

		num ans = 1;
		for (int e = 0; e < N-1; e++) {
			//cerr << edges[e].second[0] << ".." << edges[e].second[1] << ' ' << edges[e].first << ' ' << tot << '-' << edge_nodes[e].val << "=" << tot-edge_nodes[e].val << '\n';
			ans *= tot - edge_nodes[e].val;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
