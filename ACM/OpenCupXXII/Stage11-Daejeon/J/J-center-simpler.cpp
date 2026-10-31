#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

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

	// Link v2 as a child of v1 with edge e
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

	// Link v2's root as a child of v1 with edge e
	// Returns false if they're already in the same subtree
	friend bool link_root(top_tree_node* e, top_tree_node* v1, top_tree_node* v2) {
		assert(e && v1 && v2);
		assert(!e->c[0] && !e->c[1] && !e->c[2]);
		v1->expose();
		v2->expose();

		while (v1->p) v1 = v1->p;
		while (v2->p) v2 = v2->p;
		if (v1 == v2) return false;

		assert(!v1->p);
		assert(!v2->p);

		e->is_path = true, e->is_vert = false;
		e->c[0] = v1;
		v1->p = e;
		e->c[1] = v2;
		v2->p = e;
		e->update();

		return true;
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

struct top_tree_node : public top_tree_node_base<top_tree_node> {
	bool lazy_flip_path = false;

	int idx;
	// path length in edges
	int path_len = 0;
	std::pair<int, std::array<int, 2>> diam{-1, {-1, -1}};
	std::array<std::pair<int, int>, 2> max_path_len{std::pair<int, int>{-1, -1}, {-1, -1}};

	// these guys both currently have length max_path_len[z]
	std::array<num, 2> no_A_1_vert;
	// ordered 2 verts
	std::array<num, 2> no_A_2_vert;

	// For these, the A is implicitly the rake node
	num no_A_1_path;

	std::array<num, 2> path_A_1_vert;
	// ordered 2 verts
	std::array<num, 2> path_A_2_vert;

	// ordered, counts both {x, y}, {a, ?} and {x, ?}, {a, b}
	num path_A_1_path_1_vert;
	num path_A_2_path;

	// For rake nodes, the following ones matter
	// Here, having no A and a path guarantees that A is the rake node
	// The endpoints of the paths are not allowed to be A
	// max_path_len[0]
	// no_A_1_vert[0]
	// no_A_2_vert[0]
	// no_A_1_path
	// path_A_2_vert[0]
	// path_A_1_path_1_vert
	// path_A_2_path

	// For compress nodes, the following ones matter
	// Here, having only 1 vert guarantees that A is on the path
	// max_path_len[z]
	// no_A_none (implicit)
	// no_A_1_vert[z]
	// no_A_2_vert[z]
	// path_A_none (implicit)
	// path_A_1_vert[z]
	// path_A_2_vert[z]

	void do_flip_path() {
		assert(is_path);
		std::swap(c[0], c[1]);
		lazy_flip_path ^= 1;

		std::swap(max_path_len[0], max_path_len[1]);
		std::swap(no_A_1_vert[0], no_A_1_vert[1]);
		std::swap(no_A_2_vert[0], no_A_2_vert[1]);
		std::swap(path_A_1_vert[0], path_A_1_vert[1]);
		std::swap(path_A_2_vert[0], path_A_2_vert[1]);
	}

	void downdate() {
		if (lazy_flip_path) {
			assert(is_path);
			if (!is_vert) {
				assert(c[0] && c[1]);
				c[0]->do_flip_path();
				c[1]->do_flip_path();
			}
			lazy_flip_path = false;
		}
	}

	void update() {
		auto check_diam = [&](std::pair<int, int> a, std::pair<int, int> b, int constant) {
			setmax(diam, {a.first + constant + b.first, {a.second, b.second}});
		};
		assert(!lazy_flip_path);
		if (is_path && !is_vert) {
			path_len = c[0]->path_len + 1 + c[1]->path_len;
			max_path_len[0] = std::max(c[0]->max_path_len[0], {c[0]->path_len + 1 + c[1]->max_path_len[0].first, c[1]->max_path_len[0].second});
			max_path_len[1] = std::max(c[1]->max_path_len[1], {c[1]->path_len + 1 + c[0]->max_path_len[1].first, c[0]->max_path_len[1].second});
			diam = std::max(c[0]->diam, c[1]->diam);
			check_diam(c[0]->max_path_len[1], c[1]->max_path_len[0], 1);

			for (int z = 0; z < 2; z++) {
				bool z_good = (max_path_len[z].first == c[z]->max_path_len[z].first);
				bool nz_good = (max_path_len[z].first == c[z]->path_len + 1 + c[!z]->max_path_len[z].first);

				no_A_1_vert[z] = 0;
				if (z_good) no_A_1_vert[z] += c[z]->no_A_1_vert[z];
				if (nz_good) no_A_1_vert[z] += c[!z]->no_A_1_vert[z];

				no_A_2_vert[z] = 0;
				if (z_good) no_A_2_vert[z] += c[z]->no_A_2_vert[z];
				if (nz_good) no_A_2_vert[z] += c[!z]->no_A_2_vert[z];
				if (z_good && nz_good) no_A_2_vert[z] += 2 * c[z]->no_A_1_vert[z] * c[!z]->no_A_1_vert[z];

				path_A_1_vert[z] = 0;
				if (z_good) path_A_1_vert[z] += c[z]->path_A_1_vert[z];
				if (nz_good) path_A_1_vert[z] += c[!z]->path_A_1_vert[z];
				if (nz_good) path_A_1_vert[z] += num(c[z]->path_len + 1) * c[!z]->no_A_1_vert[z];

				path_A_2_vert[z] = 0;
				if (z_good) path_A_2_vert[z] += c[z]->path_A_2_vert[z];
				if (nz_good) path_A_2_vert[z] += c[!z]->path_A_2_vert[z];
				if (nz_good) path_A_2_vert[z] += num(c[z]->path_len + 1) * c[!z]->no_A_2_vert[z];
				if (z_good && nz_good) path_A_2_vert[z] += 2 * c[z]->path_A_1_vert[z] * c[!z]->no_A_1_vert[z];
			}
		} else {
			if (!is_vert) {
				// rake
				max_path_len[0] = {1 + c[2]->max_path_len[0].first, c[2]->max_path_len[0].second};
				diam = c[2]->diam;

				no_A_1_vert[0] = c[2]->no_A_1_vert[0];
				no_A_2_vert[0] = c[2]->no_A_2_vert[0];
				no_A_1_path = 0;
				path_A_2_vert[0] = c[2]->path_A_2_vert[0];
				path_A_1_path_1_vert = 0;
				path_A_2_path = 0;
			} else {
				// rake + convert to path
				max_path_len[0] = {0, idx};
				diam = {0, {idx, idx}};

				// The designated leaves may be ourself
				no_A_1_vert[0] = 1;
				no_A_2_vert[0] = 1;
				no_A_1_path = 1;
				path_A_2_vert[0] = 0;
				path_A_1_path_1_vert = 0;
				path_A_2_path = 0;
			}

			for (int z = 0; z < 2; z++) {
				if (!c[z]) continue;
				// First, set diam regardless
				check_diam(max_path_len[0], c[z]->max_path_len[0], 0);
				setmax(diam, c[z]->diam);

				if (c[z]->max_path_len[0].first < max_path_len[0].first) {
					// This branch is useless
					continue;
				} else if (c[z]->max_path_len[0].first > max_path_len[0].first){
					// Our original stuff is useless
					max_path_len[0] = c[z]->max_path_len[0];

					no_A_1_vert[0] = c[z]->no_A_1_vert[0];
					no_A_2_vert[0] = c[z]->no_A_2_vert[0];
					no_A_1_path = c[z]->no_A_1_path;
					path_A_2_vert[0] = c[z]->path_A_2_vert[0];
					path_A_1_path_1_vert = c[z]->path_A_1_path_1_vert;
					path_A_2_path = c[z]->path_A_2_path;
				} else {
					// merge the 2 sides
					// go from biggest down

					// 4 on the same side
					path_A_2_path = path_A_2_path + c[z]->path_A_2_path;
					// 3 on 1 side, 1 on the other
					path_A_2_path += path_A_1_path_1_vert * c[z]->no_A_1_vert[0];
					path_A_2_path += c[z]->path_A_1_path_1_vert * no_A_1_vert[0];
					// just 2 verts on each side
					path_A_2_path += path_A_2_vert[0] * c[z]->no_A_2_vert[0];
					path_A_2_path += c[z]->path_A_2_vert[0] * no_A_2_vert[0];

					path_A_1_path_1_vert = path_A_1_path_1_vert + c[z]->path_A_1_path_1_vert;
					path_A_1_path_1_vert += 2 * path_A_2_vert[0] * c[z]->no_A_1_vert[0];
					path_A_1_path_1_vert += 2 * c[z]->path_A_2_vert[0] * no_A_1_vert[0];

					path_A_2_vert[0] = path_A_2_vert[0] + c[z]->path_A_2_vert[0];

					no_A_1_path = no_A_1_path + c[z]->no_A_1_path;
					no_A_1_path += no_A_1_vert[0] * c[z]->no_A_1_vert[0];

					no_A_2_vert[0] = no_A_2_vert[0] + c[z]->no_A_2_vert[0];
					no_A_2_vert[0] += 2 * no_A_1_vert[0] * c[z]->no_A_1_vert[0];

					no_A_1_vert[0] += c[z]->no_A_1_vert[0];
				}
			}

			if (is_vert) {
				path_len = 0;

				// the A must be ourself
				path_A_1_vert[0] = no_A_1_vert[0];
				path_A_2_vert[0] += no_A_2_vert[0];

				// Copy things in both directions
				max_path_len[1] = max_path_len[0];
				no_A_1_vert[1] = no_A_1_vert[0];
				no_A_2_vert[1] = no_A_2_vert[0];
				path_A_1_vert[1] = path_A_1_vert[0];
				path_A_2_vert[1] = path_A_2_vert[0];
			}
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, Q; cin >> N >> M >> Q;

	std::vector<top_tree_node> verts(N);
	for (int i = 0; i < N; i++) {
		auto n = &verts[i];
		n->idx = i;
		n->is_path = n->is_vert = true;
		n->update();
	}

	std::vector<top_tree_node> edges(N-1);
	std::vector<top_tree_node*> free_list(N-1);
	for (int e = 0; e < N-1; e++) {
		free_list[e] = &edges[e];
	}

	for (int z = 0; z < M; z++) {
		int x, y; cin >> x >> y; x--, y--;
		auto e = free_list.back(); free_list.pop_back();
		link(e, &verts[x], &verts[y]);
	}

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int x, y; cin >> x >> y; x--, y--;
			auto e = free_list.back(); free_list.pop_back();
			link(e, &verts[x], &verts[y]);
		} else if (op == 2) {
			int x, y; cin >> x >> y; x--, y--;
			auto e = get_path(&verts[x], &verts[y]);
			cut(e);
			free_list.push_back(e);
		} else if (op == 3) {
			int x; cin >> x; x--;
			verts[x].make_root();
			auto [d, vs] = verts[x].diam;
			auto r = get_path(&verts[vs[0]], &verts[vs[1]]);
			// walk down the tree to either an edge or a vertex
			if (d % 2 == 0) {
				// find the center vertex
				int l = d / 2;
				while (!r->is_vert) {
					r->downdate();
					if (l > r->c[0]->path_len) {
						l -= r->c[0]->path_len + 1;
						r = r->c[1];
					} else {
						r = r->c[0];
					}
				}
				assert(l == 0);
				r = get_path(r, r);
				cout << r->path_A_2_path + r->no_A_1_path * r->no_A_1_path << '\n';
			} else {
				// find the center edge
				int l = (d-1) / 2;
				while (r->c[0]->path_len != l) {
					r->downdate();
					if (l > r->c[0]->path_len) {
						l -= r->c[0]->path_len + 1;
						r = r->c[1];
					} else {
						r = r->c[0];
					}
				}
				r->expose_edge();
				r->downdate();
				cout << (r->c[0]->no_A_2_vert[1] * r->c[1]->path_A_2_vert[0] + r->c[0]->path_A_2_vert[1] * r->c[1]->no_A_2_vert[0]) << '\n';
			}
		}
	}

	return 0;
}
