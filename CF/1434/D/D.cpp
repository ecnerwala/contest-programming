#include<bits/stdc++.h>

using std::swap;

template <typename T> T& setmax(T& a, const T& b) { if (b > a) a = b; return a; }
template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

const int INF = 1e9;

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

	bool own_parity;

	int path_len;
	bool path_parity;
	int best_path;
	std::array<int, 2> best_up;
	std::array<int, 2> best_down;

	void do_flip_path() {
		assert(is_path);
		flip_path ^= 1;
		swap(best_up, best_down);
	}

	void do_flip_edge_parity() {
		assert(!is_vert);
		own_parity ^= 1;
		update();
	}

	void downdate() {
		if (flip_path) {
			assert(is_path);
			if (!is_vert) {
				if (c[0]) c[0]->do_flip_path();
				if (c[1]) c[1]->do_flip_path();
			}
			swap(c[0], c[1]);
			flip_path = false;
		}
	}

	void update() {
		if (is_path && !is_vert) {
			assert(!c[2]);
			assert(c[0] && c[1]);

			// path edge
			path_len = c[0]->path_len + 1 + c[1]->path_len;
			path_parity = c[0]->path_parity ^ own_parity ^ c[1]->path_parity;
			best_up = c[0]->best_up;
			for (int z = 0; z < 2; z++) {
				setmax(best_up[c[0]->path_parity ^ own_parity ^ z], c[0]->path_len + 1 + c[1]->best_up[z]);
			}
			best_down = c[1]->best_down;
			for (int z = 0; z < 2; z++) {
				setmax(best_down[c[1]->path_parity ^ own_parity ^ z], c[1]->path_len + 1 + c[0]->best_down[z]);
			}
			best_path = std::max(c[0]->best_path, c[1]->best_path);
			for (int z = 0; z < 2; z++) {
				setmax(best_path, c[0]->best_down[z] + 1 + c[1]->best_up[z ^ own_parity]);
			}
		} else {
			assert((is_vert && is_path) || (!is_vert && !is_path));
			if (is_vert) {
				path_len = 0;
				path_parity = 0;
				best_path = 0;
				best_up = {0, -INF};
			} else {
				best_path = c[2]->best_path;
				best_up = c[2]->best_up;

				// add yourself
				if (own_parity) {
					swap(best_up[0], best_up[1]);
				}
				best_up[0]++, best_up[1]++;
				setmax(best_path, best_up[0]);
			}

			for (int d = 0; d < 2; d++) {
				if (!c[d]) continue;
				setmax(best_path, c[d]->best_path);
				for (int z = 0; z < 2; z++) {
					setmax(best_path, c[d]->best_up[z] + best_up[z]);
					setmax(best_up[z], c[d]->best_up[z]);
				}
			}
			best_down = best_up;
		}

		if (flip_path) {
			assert(is_path);
			swap(best_up, best_down);
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

	friend std::pair<top_tree_node*, top_tree_node*> cut(top_tree_node* e) {
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
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;

	vector<top_tree_node> nodes(N+(N-1));

	for (int i = 0; i < N; i++) {
		top_tree_node* n = &nodes[i];
		n->is_path = n->is_vert = true;
		n->update();
	}

	vector<array<int, 2>> edges(N-1);
	for (int e = 0; e < N-1; e++) {
		int u, v, t; cin >> u >> v >> t; u--, v--;
		edges[e] = {u, v};

		nodes[N+e].own_parity = t;
		link(&nodes[N+e], &nodes[u], &nodes[v]);
	}

	int Q; cin >> Q;
	while (Q--) {
		int e; cin >> e; e--;
		auto [u,v] = edges[e];
		auto pth = get_path(&nodes[u], &nodes[v]);
		assert(pth == &nodes[N+e]);
		pth->do_flip_edge_parity();
		cout << pth->best_path << '\n';
	}

	return 0;
}
