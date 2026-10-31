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

struct my_top_tree_node : public top_tree_node_base<my_top_tree_node> {
	bool lazy_flip_path = false;

	char edge_val;
	std::bitset<676> down_reqs;
	std::bitset<676> up_reqs;
	int head_opts;
	int tail_opts;
	int path_len;

	void do_flip_path() {
		assert(is_path);
		std::swap(c[0], c[1]);
		std::swap(down_reqs, up_reqs);
		std::swap(head_opts, tail_opts);
		lazy_flip_path ^= 1;
	}

	void downdate() {
		if (lazy_flip_path) {
			assert(is_path);
			if (!is_vert) {
				c[0]->do_flip_path();
				c[1]->do_flip_path();
			}
			lazy_flip_path = false;
		}
	}

	std::bitset<676> own_down_reqs() {
		return (std::bitset<676>(c[0]->tail_opts) << (26 * (edge_val - 'A')));
	}

	// NOTE: You may assume downdate() has been called on the current node, but
	// it may not have been called on the children! In particular, be careful
	// when accessing grandchildren information.
	void update() {
		if (is_vert) {
			assert(is_path);
			head_opts = (c[0] ? c[0]->head_opts : 0) | (c[1] ? c[1]->head_opts : 0);
			tail_opts = head_opts;
			path_len = 0;
		} else if (is_path) {
			head_opts = c[0]->head_opts;
			tail_opts = c[1]->tail_opts;
			path_len = 1 + c[0]->path_len + c[1]->path_len;
			down_reqs = c[0]->down_reqs | c[1]->down_reqs | (std::bitset<676>(c[0]->tail_opts) << (26 * (edge_val - 'A')));
			up_reqs = c[0]->up_reqs | c[1]->up_reqs | (std::bitset<676>(c[1]->head_opts) << (26 * (edge_val - 'A')));
		} else {
			head_opts = (c[0] ? c[0]->head_opts : 0) | (c[1] ? c[1]->head_opts : 0) | (1 << (edge_val - 'A'));
			tail_opts = head_opts;
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<std::tuple<int, int, char>> init_edges(N-1);
		for (auto& [a, b, c] : init_edges) {
			cin >> a >> b >> c; a--, b--;
		}
		int Q; cin >> Q;

		cerr << "Case #" << case_num << " N = " << N << " Q = " << Q << '\n';

		std::vector<my_top_tree_node> nodes(2*(N+Q));
		int nxt_v = N;
		int nxt_e = N+Q;
		for (int i = 0; i < N; i++) {
			nodes[i].is_path = nodes[i].is_vert = true;
			nodes[i].update();
		}
		for (auto [a, b, c] : init_edges) {
			auto n = &nodes[nxt_e++];
			n->edge_val = c;
			link(n, &nodes[a], &nodes[b]);
			//cerr << "link " << (n - nodes.data()) << ' ' << a << ' ' << b << '\n';
		}

		std::vector<int> all_ans; all_ans.reserve(Q);

		auto dump_bs = [](std::bitset<676> bs) {
			for (int i = 0; i < 26; i++) {
				for (int j = 0; j < 26; j++) {
					if (bs[i*26+j]) {
						cerr << char('A'+i) << '<' << char('A'+j) << ' ';
					}
				}
			}
			cerr << '\n';
		};
		for (int q = 0; q < Q; q++) {
			int op; cin >> op;
			//cerr << "op " << q << ' ' << op << '\n';
			if (op == 1) {
				int u; cin >> u; u--;
				char c; cin >> c;

				auto a = &nodes[u];

				auto b = &nodes[nxt_v++];
				b->is_path = b->is_vert = true;
				b->update();

				auto e = &nodes[nxt_e++];

				e->edge_val = c;
				link(e, a, b);
			} else if (op == 2) {
				int u, k; string s; cin >> u >> k >> s; u--;
				//cerr << u << ' ' << k << ' ' << s << '\n';

				std::bitset<676> reqs;
				for (int i = 0; i < int(s.size()); i++) {
					for (int j = i+1; j < int(s.size()); j++) {
						reqs.set((s[i] - 'A') * 26 + (s[j] - 'A'));
					}
				}

				auto n = &nodes[u];
				n->make_root();
				while (true) {
					n->downdate();
					assert(!n->lazy_flip_path);
					//cerr << " at node " << n-nodes.data() << ' ' << n->is_path << ' ' << n->is_vert << '\n';
					if (n->is_path && !n->is_vert) {
						//cerr << "  " << n->c[0]-nodes.data() << ' ' << n->c[1]-nodes.data() << '\n';
						if (k <= n->c[0]->path_len) {
							//cerr << "  going left cause len" << '\n';
							n = n->c[0];
							continue;
						}
						if (((n->c[0]->down_reqs | n->own_down_reqs()) & ~reqs).any()) {
							//cerr << "  going left cause reqs" << '\n';
							//cerr << n->c[0]->down_reqs << '\n';
							//cerr << "own " << '\n'; dump_bs(n->own_down_reqs());
							// we break already in n->c[0] or ourself, so go left
							n = n->c[0];
							continue;
						}
						//cerr << "  jumping k " << n->c[0]->path_len + 1 << '\n';
						k -= n->c[0]->path_len + 1;
						n = n->c[1];
					} else {
						if (n->is_vert && (k == 0 || !(n->c[0] || n->c[1]))) {
							// this is the end
							if (k == 0) {
								//cerr << "  got to limit" << '\n';
							} else {
								//cerr << "  got to leaf" << '\n';
							}
							break;
						}
						// find the first guy
						for (char c : s) {
							if (!n->is_vert && c == n->edge_val) {
								//cerr << "  jumping light " << n-nodes.data() << '\n';
								k--;
								n = n->c[2];
								goto found;
							}
							if (n->c[1] && (n->c[1]->head_opts & (1 << (c - 'A')))) {
								n = n->c[1];
								goto found;
							}
							if (n->c[0] && (n->c[0]->head_opts & (1 << (c - 'A')))) {
								n = n->c[0];
								goto found;
							}
						}
						assert(false);
found:;
					}
				}
				// splay it to fix amortization
				assert(k >= 0);
				int ans = int(n - &nodes[0]);
				n->expose();
				all_ans.push_back(ans);
			}
		}

		cout << "Case #" << case_num << ":";
		for (int z = 0; z < int(all_ans.size()); z++) {
			cout << ' ' << all_ans[z]+1;
		}
		cout << '\n';
	}

	return 0;
}
