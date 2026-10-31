#include<bits/stdc++.h>
using namespace std;

template <typename T> T& setmax(T& a, const T& b) { if (b > a) a = b; return a; }
template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

struct range_data {
	int sz;
	int max_val;
	int min_val;
	int sum;
	int lazy_add;
	bool lazy_set; // if true, lazy_add means set instead of +=

	range_data() : sz(0), max_val(INT_MIN), min_val(INT_MAX), sum(0), lazy_add(0), lazy_set(false) {}
	explicit range_data(int v) : sz(1), max_val(v), min_val(v), sum(v), lazy_add(0), lazy_set(false) {}
	range_data(const range_data& a, const range_data& b) : range_data(a) {
		update_with(b);
	}

	void apply_add(int v) {
		lazy_add += v;
		sum += sz * v;
		max_val += v;
		min_val += v;
	}

	void apply_set(int v) {
		lazy_set = true;
		lazy_add = v;
		if (sz) {
			sum = sz * lazy_add;
			min_val = max_val = lazy_add;
		}
	}

	void update_with(const range_data& o) {
		assert(lazy_add == 0 && lazy_set == false);
		sz += o.sz;
		setmax(max_val, o.max_val);
		setmin(min_val, o.min_val);
		sum += o.sum;
	}

	void downdate_to(range_data& o) {
		if (lazy_set) {
			o.apply_set(lazy_add);
		} else if (lazy_add) {
			o.apply_add(lazy_add);
		}
	}

	void clear_downdate() {
		lazy_set = false;
		lazy_add = 0;
	}
};

struct top_tree_node {
	mutable top_tree_node* p = nullptr;
	top_tree_node* c[3] = {nullptr, nullptr, nullptr};

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

	bool flip_path = false;

	range_data own_val;
	range_data path_val;
	range_data strict_subtree_val;

	void do_flip_path() {
		assert(is_path);
		flip_path ^= 1;
	}

	void do_path_set(int v) {
		path_val.apply_set(v);
	}

	void do_path_add(int v) {
		path_val.apply_add(v);
	}

	void do_subtree_set(int v) {
		path_val.apply_set(v);
		strict_subtree_val.apply_set(v);
	}

	void do_subtree_add(int v) {
		path_val.apply_add(v);
		strict_subtree_val.apply_add(v);
	}

	range_data subtree_val() {
		return range_data(path_val, strict_subtree_val);
	}

	void downdate() {
		// fortunately, this doesn't interact with our ops
		if (flip_path) {
			assert(is_path);
			if (!is_vert) {
				if (c[0]) c[0]->do_flip_path();
				if (c[1]) c[1]->do_flip_path();
			}
			swap(c[0], c[1]);
			flip_path = false;
		}

		if (is_path) {
			path_val.downdate_to(own_val);
			if (!is_vert) {
				assert(c[0] && c[1]);
				path_val.downdate_to(c[0]->path_val);
				path_val.downdate_to(c[1]->path_val);
			}
		}
		path_val.clear_downdate();

		if (!is_path) {
			strict_subtree_val.downdate_to(own_val);

			assert(c[2]);
			strict_subtree_val.downdate_to(c[2]->path_val);
			strict_subtree_val.downdate_to(c[2]->strict_subtree_val);
		}
		if (c[0]) strict_subtree_val.downdate_to(c[0]->strict_subtree_val);
		if (c[1]) strict_subtree_val.downdate_to(c[1]->strict_subtree_val);
		strict_subtree_val.clear_downdate();

		own_val.clear_downdate();
	}

	void update() {
		if (is_path) {
			path_val = own_val;
			strict_subtree_val = range_data();
			if (!is_vert) {
				path_val.update_with(c[0]->path_val);
				path_val.update_with(c[1]->path_val);
			}
			if (c[0]) {
				strict_subtree_val.update_with(c[0]->strict_subtree_val);
			}
			if (c[1]) {
				strict_subtree_val.update_with(c[1]->strict_subtree_val);
			}
		} else {
			strict_subtree_val = own_val;
			path_val = range_data();
			if (c[0]) {
				strict_subtree_val.update_with(c[0]->strict_subtree_val);
			}
			if (c[1]) {
				strict_subtree_val.update_with(c[1]->strict_subtree_val);
			}
			assert(c[2]);
			strict_subtree_val.update_with(c[2]->strict_subtree_val);
			strict_subtree_val.update_with(c[2]->path_val);
		}
	}

	void downdate_all() {
		if (p) p->downdate_all();
		downdate();
	}

	void update_all() {
		update();
		if (p) p->update_all();
	}

private:

	void rot() {
		assert(!is_vert);
		assert(!r());
		top_tree_node* pa = p;
		int x = d(); assert(x == 0 || x == 1);
		top_tree_node* ch = c[!x];

		if (pa->p) pa->p_c() = this;
		this->p = pa->p;

		pa->c[x] = ch;
		if (ch) ch->p = pa;

		this->c[!x] = pa;
		pa->p = this;

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

		if (pa->p) pa->p_c() = this;
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

		if (pa->p) pa->p_c() = this;
		this->p = pa->p;

		pa->is_path = false;
		pa->c[2] = pa->c[1]; // don't need to change the parent

		pa->c[0] = c[0]; if (c[0]) c[0]->p = pa;
		pa->c[1] = c[1]; if (c[1]) c[1]->p = pa;

		c[0] = nullptr;
		c[1] = pa; pa->p = this;
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

		if (pa->p) pa->p_c() = this;
		this->p = pa->p;

		pa->c[0] = c[0]; if (c[0]) c[0]->p = pa;
		pa->c[1] = c[1]; if (c[1]) c[1]->p = pa;

		assert(c[2] && c[2]->is_path);
		c[1] = c[2]; // don't need to change parent
		c[0] = pa; pa->p = this;
		c[2] = nullptr;

		is_path = true;

		pa->update();
		return pa;
	}

	void splice_all(top_tree_node*& res) {
		if (!is_path) {
			res = splice_non_path();
		}
		assert(is_path);
		if (!p) return;
		p->splice_all(res);
	}

public:
	top_tree_node* expose() {
		assert(is_vert);
		downdate_all();

		top_tree_node* res = nullptr;
		splice_all(res);

		cut_right();

		update_all();

		return res;
	}

	void meld_path_end() {
		assert(!p);
		top_tree_node* rt = this;
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
		rt->update_all();
	}

	void make_root() {
		expose();

		top_tree_node* rt = this;
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

	friend pair<top_tree_node*, top_tree_node*> cut(top_tree_node* e) {
		assert(!e->p);
		assert(e->is_path);
		assert(!e->is_vert);

		e->downdate();

		top_tree_node* l = e->c[0];
		top_tree_node* r = e->c[1];
		assert(l && r);

		e->c[0] = e->c[1] = nullptr;
		l->p = r->p = nullptr;

		assert(e->c[2] == nullptr);

		l->meld_path_end();

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

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N, Q; cin >> N >> Q;

	vector<top_tree_node> nodes(N+(N-1));

	auto scan_node_index = []() -> int { int x; cin >> x; x--; return x; };
	auto scan_node = [&]() -> top_tree_node* { return &nodes[scan_node_index()]; };
	auto scan_value = []() -> int { int v; cin >> v; return v; };

	for (int i = 0; i < N; i++) {
		top_tree_node* n = &nodes[i];
		n->is_path = n->is_vert = true;
		n->own_val = range_data(0);
		n->update();
	}

	for (int e = 0; e < N-1; e++) {
		top_tree_node* nx = scan_node();
		top_tree_node* ny = scan_node();

		top_tree_node* ne = &nodes[N+e];

		link(ne, nx, ny);
	}

	//for (auto& n : nodes) { n.check_state(); }

	for (int i = 0; i < N; i++) {
		int w = scan_value();
		top_tree_node* n = &nodes[i];
		n->make_root();
		n->own_val = range_data(w);
		n->update();
		//for (auto& n : nodes) { n.check_state(); }
	}

	top_tree_node* cur_root = scan_node();

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		switch (op) {
			case 0:
				{
					top_tree_node* x = scan_node();
					int v = scan_value();

					top_tree_node* subtree = get_subtree(cur_root, x);

					subtree->do_subtree_set(v);
					subtree->downdate();
					subtree->update_all();
				}
				break;
			case 1:
				{
					top_tree_node* x = scan_node();

					cur_root = x;
				}
				break;
			case 2:
				{
					top_tree_node* x = scan_node();
					top_tree_node* y = scan_node();
					int v = scan_value();

					top_tree_node* path = get_path(x, y);

					path->do_path_set(v);
					path->downdate();
					path->update_all();
				}
				break;
			case 3:
				{
					top_tree_node* x = scan_node();

					top_tree_node* subtree = get_subtree(cur_root, x);

					cout << subtree->subtree_val().min_val << '\n';
				}
				break;
			case 4:
				{
					top_tree_node* x = scan_node();

					top_tree_node* subtree = get_subtree(cur_root, x);

					cout << subtree->subtree_val().max_val << '\n';
				}
				break;
			case 5:
				{
					top_tree_node* x = scan_node();
					int v = scan_value();

					top_tree_node* subtree = get_subtree(cur_root, x);

					subtree->do_subtree_add(v);
					subtree->downdate();
					subtree->update_all();
				}
				break;
			case 6:
				{
					top_tree_node* x = scan_node();
					top_tree_node* y = scan_node();
					int v = scan_value();

					top_tree_node* path = get_path(x, y);

					path->do_path_add(v);
					path->downdate();
					path->update_all();
				}
				break;
			case 7:
				{
					top_tree_node* x = scan_node();
					top_tree_node* y = scan_node();

					top_tree_node* path = get_path(x, y);

					cout << path->path_val.min_val << '\n';
				}
				break;
			case 8:
				{
					top_tree_node* x = scan_node();
					top_tree_node* y = scan_node();

					top_tree_node* path = get_path(x, y);

					cout << path->path_val.max_val << '\n';
				}
				break;
			case 9:
				{
					top_tree_node* x = scan_node();
					top_tree_node* y = scan_node();

					cur_root->make_root();
					y->expose();
					top_tree_node* exp = x->expose();
					if (exp == nullptr) {
						// we can't do this, noop
					} else {
						top_tree_node* e = x->p;
						cut(e);
						link(e, y, x);
					}
				}
				break;
			case 10:
				{
					top_tree_node* x = scan_node();
					top_tree_node* y = scan_node();

					top_tree_node* path = get_path(x, y);

					cout << path->path_val.sum << '\n';
				}
				break;
			case 11:
				{
					top_tree_node* x = scan_node();

					top_tree_node* subtree = get_subtree(cur_root, x);

					cout << subtree->subtree_val().sum << '\n';
				}
				break;
			default: assert(false);
		}
	}

	return 0;
}
