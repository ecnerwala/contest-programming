#include <bits/stdc++.h>

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

	friend top_tree_node* get_path_to_root(top_tree_node* b) {
		assert(b->is_vert);
		b->expose();
		if (!b->p) return b;
		assert(!b->p->p);
		return b->p;
	}

	friend top_tree_node* get_subtree_from_root(top_tree_node* n) {
		n->expose();
		return n;
	}

};

template <typename T> void setmin(T& a, T b) {
	if (b < a) a = b;
}

const int64_t INF = 1e18;
std::array<int64_t, 8> sum_array(std::array<int64_t, 8> a, std::array<int64_t, 8> b) {
	std::array<int64_t, 8> res; res.fill(INF);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			setmin(res[i|j], a[i] + b[j]);
		}
	}
	return res;
}

struct sample_top_tree_node;
sample_top_tree_node* base_ptr = nullptr;

struct sample_top_tree_node : public top_tree_node_base<sample_top_tree_node> {
	//bool lazy_flip_path = false;

	void do_flip_path() {
		assert(false);
		/*
		assert(is_path);
		std::swap(c[0], c[1]);
		lazy_flip_path ^= 1;
		*/
	}

	void downdate() {
		/*
		if (lazy_flip_path) {
			assert(is_path);
			if (!is_vert) {
				c[0]->do_flip_path();
				c[1]->do_flip_path();
			}
			lazy_flip_path = false;
		}
		*/
	}

	std::array<std::array<int64_t, 8>, 8> values;
	int64_t own_b;
	int64_t own_g;
	int64_t own_h;

	const int GLEFT = 1;
	const int GRIGHT = 2;
	const int HLEFT = 4;

	// NOTE: You may assume downdate() has been called on the current node, but
	// it may not have been called on the children! In particular, be careful
	// when accessing grandchildren information.
	void update() {
		if (is_vert) {
			std::array<int64_t, 8> own_val; own_val.fill(INF); own_val[0] = 0;
			for (int z = 0; z < 2; z++) {
				if (c[z]) {
					own_val = sum_array(own_val, c[z]->values[0]);
				}
			}
			for (int i = 0; i < 8; i++) {
				std::array<int64_t, 8> cnd_val; cnd_val.fill(INF);
				cnd_val[i] = 0;
				cnd_val = sum_array(cnd_val, own_val);

				values[i].fill(INF);
				for (int j = 0; j < 8; j++) {
					// o-right
					if (!(j & GLEFT)) {
						// then g-right, and might as well keep hleft
						if (i == 0 && j == 0) {
							//std::cerr << "vert " << this-base_ptr << ' ' << i << ' ' << j << ' ' << cnd_val[j] << ' ' << own_b << ' ' << own_g << '\n';
						}
						setmin(values[i][(j & HLEFT) | GRIGHT], cnd_val[j] - own_b - ((j & HLEFT) ? 0 : own_h) + own_g);
					}
					{
						// o-left implies hleft
						if (!(j & GRIGHT)) {
							// g-left
							setmin(values[i][HLEFT | GLEFT], cnd_val[j]);
						}

						{
							// g-right
							setmin(values[i][HLEFT | GRIGHT], cnd_val[j] + own_g);
						}
					}
				}
			}
		} else if (is_path) {
			// just matrix multiply, our indices go bottom to top
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					int64_t v = INF;
					for (int k = 0; k < 8; k++) {
						setmin(v, c[1]->values[i][k] + c[0]->values[k][j]);
					}
					values[i][j] = v;
				}
			}
		} else {
			// We're an edge
			std::array<int64_t, 8> own_val = c[2]->values[0];
			for (int z = 0; z < 2; z++) {
				if (c[z]) {
					own_val = sum_array(own_val, c[z]->values[0]);
				}
			}
			values[0] = own_val;
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		// We're computing the min (cut - b_i - h_i), and if it's 0 then it's doable
		int N, Q; cin >> N >> Q;
		cerr << "Start case " << "N = " << N << ' ' << "Q = " << Q << '\n';
		std::vector<int> par(N);
		par[0] = -1;
		for (int i = 1; i < N; i++) cin >> par[i];
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;

		std::vector<sample_top_tree_node> nodes(2*N-1);
		base_ptr = nodes.data();
		for (int i = 0; i < N; i++) {
			nodes[i].is_path = nodes[i].is_vert = true;

			nodes[i].own_b = B[i];
			nodes[i].own_g = 0;
			nodes[i].own_h = 0;

			nodes[i].update();
		}
		for (int i = N-1; i >= 1; i--) {
			auto e = &nodes[N+i-1];

			link_root(e, &nodes[par[i]], &nodes[i]);
		}
		for (int v = 0; v < N; v++) {
			cerr << "Node " << v << '\n';
			nodes[v].expose();
			for (int i = 0; i < 8; i++) {
				cerr << nodes[v].values[0][i] << ' ';
			}
			cerr << '\n';
		}
		for (int v = 0; v < N; v++) {
			cerr << "Node " << v << '\n';
			nodes[v].expose();
			for (int i = 0; i < 8; i++) {
				cerr << nodes[v].values[0][i] << ' ';
			}
			cerr << '\n';
		}
		for (int q = 0; q < Q; q++) {
			int t, v, delta; cin >> t >> v >> delta; v--;
			cerr << "op " << t << ' ' << v << ' ' << delta << '\n';
			nodes[v].expose();
			if (t == 1) {
				nodes[v].own_g += delta;
			} else if (t == 2) {
				nodes[v].own_h += delta;
			} else assert(false);
			auto rt = nodes[v].update_all();
			int64_t res = *std::min_element(rt->values[0].begin(), rt->values[0].end());
			cerr << "res " << res << '\n';
			assert(res <= 0);
			if (res < 0) {
				cout << "NO" << '\n';
			} else {
				cout << "YES" << '\n';
			}
		}
	}

	return 0;
}
