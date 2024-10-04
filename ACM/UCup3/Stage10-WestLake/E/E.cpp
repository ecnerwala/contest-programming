#include <bits/stdc++.h>

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

template <typename splay_node> struct splay_node_base {
private:
	splay_node* derived_this() {
		return static_cast<splay_node*>(this);
	}
	const splay_node* derived_this() const {
		return static_cast<const splay_node*>(this);
	}
public:
	mutable splay_node* p = nullptr;
	std::array<splay_node*, 2> c{nullptr, nullptr};

	int d() const {
		assert(p);
		if (this == p->c[0]) return 0;
		else if (this == p->c[1]) return 1;
		else assert(false);
	}
	splay_node*& p_c() const { return p->c[d()]; }

private:
	// Convenience wrappers for the derived functions.
	void downdate() {
		derived_this()->downdate();
	}
	void update() {
		derived_this()->update();
	}

public:
	void rot() {
		assert(p);
		splay_node* pa = p;
		int x = d(); assert(x == 0 || x == 1);
		splay_node* ch = c[!x];

		if (pa->p) pa->p_c() = derived_this();
		this->p = pa->p;

		pa->c[x] = ch;
		if (ch) ch->p = pa;

		this->c[!x] = pa;
		pa->p = derived_this();

		pa->update();
	}

	splay_node* splay_no_update() {
		while (p) {
			if (p->p) {
				if (p->d() == d()) p->rot();
				else rot();
			}
			rot();
		}
		return derived_this();
	}

	splay_node* splay() {
		splay_no_update();
		update();
		return derived_this();
	}
};

// We have splay trees of child edges, as well as splay trees of vertices
// The vertex splay trees are just there to store the interleaving counts for us
struct edge_splay_node;

struct vertex_splay_node : splay_node_base<vertex_splay_node> {
	edge_splay_node* spare = nullptr;
	std::array<int, 2> own_cnt{0, 0};
	std::array<int, 2> tot_cnt{0, 0};
	bool flip = false;
	void do_flip() {
		std::swap(own_cnt[0], own_cnt[1]);
		std::swap(tot_cnt[0], tot_cnt[1]);
		flip ^= 1;
	}
	void downdate() {
		if (flip) {
			if (c[0]) c[0]->do_flip();
			if (c[1]) c[1]->do_flip();
			flip = false;
		}
	}
	void update() {
		assert(!flip);
		assert(own_cnt[0] + own_cnt[1] + bool(spare) == 2);
		tot_cnt = own_cnt;
		for (int z = 0; z < 2; z++) {
			if (c[z]) {
				tot_cnt[0] += c[z]->tot_cnt[0];
				tot_cnt[1] += c[z]->tot_cnt[1];
			}
		}
	}
};

struct edge_splay_node : splay_node_base<edge_splay_node> {
	std::array<edge_splay_node*, 2> child_edges{nullptr, nullptr};
	vertex_splay_node* child_vert = nullptr;
	int own_cnt = 0;
	int own_sz = 0;
	int tot_cnt = 0;
	int tot_sz = 0;

	bool flip = false;
	void do_flip() {
		std::swap(child_edges[0], child_edges[1]);
		flip ^= 1;
	}

	bool reverse = false;
	void do_reverse() {
		std::swap(c[0], c[1]);
		reverse ^= 1;
	}

	void downdate() {
		if (reverse) {
			if (c[0]) c[0]->do_reverse();
			if (c[1]) c[1]->do_reverse();
			reverse = false;
		}
		if (flip) {
			if (c[0]) c[0]->do_flip();
			if (c[1]) c[1]->do_flip();
			if (child_vert) {
				assert(child_edges[0]);
				assert(child_edges[1]);
				child_edges[0]->do_flip();
				child_edges[1]->do_flip();
				child_vert->do_flip();
			}
			flip = false;
		}
	}

	void update() {
		assert(!reverse);
		assert(!flip);

		own_cnt = 1;
		own_sz = child_vert ? (child_edges[0]->own_sz + child_edges[1]->own_sz) : 1;

		tot_cnt = own_cnt;
		tot_sz = own_sz;
		for (int z = 0; z < 2; z++) {
			if (c[z]) {
				tot_cnt += c[z]->tot_cnt;
				tot_sz += c[z]->tot_sz;
			}
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	assert(N >= 2);

	std::vector<edge_splay_node> edge_nodes(2*N-1);
	std::vector<vertex_splay_node> vertex_nodes(N-1);
	edge_splay_node* root = std::y_combinator([&](auto self, int l, int r) -> edge_splay_node* {
		assert(l < r);
		if (r - l == 1) {
			// just a leaf
			// could allocate a vertex, but easiest not to
			edge_splay_node* e = &edge_nodes[l * 2];
			e->child_vert = nullptr;
			e->child_edges = {nullptr, nullptr};
			e->update();
			return e;
		}

		int m; cin >> m;
		assert(l < m && m < r);
		auto el = self(l, m);
		auto er = self(m, r);

		vertex_splay_node* v = &vertex_nodes[m - 1];
		// have 1 child on each side
		v->own_cnt = {1, 1};
		v->update();

		edge_splay_node* e = &edge_nodes[2 * m - 1];
		e->child_vert = v;
		e->child_edges = {el, er};
		e->update();
		return e;
	})(0, N);

	auto op = [&](int x) -> int {
		assert(0 <= x && x <= N);
		if (x == 0) {
			return 0;
		}
		if (x == N) {
			root->do_flip();
			// I don't like leaving stuff in the root
			root->downdate();
			return 1;
		}
		assert(0 < x && x < N);

		// TODO: Expose the x-1 -> x cutoff
		while (true) {
			assert(0 < x && x < root->own_sz);
			assert(root->child_vert);
			cerr << root->child_edges[0]->tot_sz << '\n';
			if (x == root->child_edges[0]->tot_sz) {
				// This is the leaf
				break;
			}
			int z;
			int target_l_sz;
			if (x < root->child_edges[0]->tot_sz) {
				z = 0;
				target_l_sz = x;
			} else {
				z = 1;
				target_l_sz = root->own_sz - x;
			}
			// nxt_sz is the number of shallow (left) things
			// TODO: Split at that location, splice in the other side
			edge_splay_node* cur_e = root->child_edges[z];
			{
				int cur_l_sz = 0;
				while (true) {
					cur_e->downdate();
					int cnd_l_sz = cur_e->c[0] ? cur_e->c[0]->tot_sz : 0;
					if (target_l_sz <= cur_l_sz + cnd_l_sz) {
						cur_e = cur_e->c[0];
						continue;
					}
					cur_l_sz += cnd_l_sz;
					cur_l_sz += cur_e->own_sz;
					if (target_l_sz <= cur_l_sz) {
						// This is the one to take
						break;
					}
					cur_e = cur_e->c[1];
				}
				cur_e->splay();
				root->child_edges[z] = cur_e;
			}

			vertex_splay_node* cur_v = root->child_vert;
			{
				int target_cnt = (cur_e->c[0] ? cur_e->c[0]->tot_cnt : 0) + 1;
				while (true) {
					assert(cur_v);
					cur_v->downdate();
					assert(1 <= target_cnt && target_cnt <= cur_v->tot_cnt[z]);
					int c0_cnt = cur_v->c[0] ? cur_v->c[0]->tot_cnt[z] : 0;
					if (target_cnt <= c0_cnt) {
						cur_v = cur_v->c[0];
						continue;
					}
					target_cnt -= c0_cnt;
					if (target_cnt <= cur_v->own_cnt[z]) {
						assert(cur_v->own_cnt[z] == 1);
						break;
					}
					target_cnt -= cur_v->own_cnt[z];
					cur_v = cur_v->c[1];
					continue;
				}
				cur_v->splay();
				root->child_vert = cur_v;
			}

			assert(cur_v->own_cnt[z]);
			if (!cur_v->own_cnt[!z]) {
				// cut the right edge
				assert(cur_v->spare);
				assert(cur_v->c[1]);
				edge_splay_node* nxt_oe = std::exchange(cur_v->spare, nullptr);
				// Clear it out for good measure
				*nxt_oe = edge_splay_node{};

				cur_v->downdate();
				nxt_oe->child_vert = cur_v->c[1];
				cur_v->c[1]->p = nullptr;
				cur_v->c[1] = nullptr;
				cur_v->own_cnt[!z] = 1;
				assert(cur_v->own_cnt[!z] == 1);
				assert(cur_v->own_cnt[z] == 1);
				assert(!cur_v->spare);
				cur_v->update();

				cur_e->downdate();
				nxt_oe->child_edges[z] = cur_e->c[1];
				if (cur_e->c[1]) cur_e->c[1]->p = nullptr;
				cur_e->c[1] = nullptr;
				cur_e->update();

				{
					int target_cnt = cur_v->c[0] ? cur_v->c[0]->tot_cnt[!z] : 0;
					if (!target_cnt) {
						// Take the whole tree
						nxt_oe->child_edges[!z] = root->child_edges[!z];
					} else {
						edge_splay_node* cur_oe = root->child_edges[!z];
						while (true) {
							cur_oe->downdate();
							int c0_cnt = cur_oe->c[0] ? cur_oe->c[0]->tot_cnt : 0;
							if (target_cnt <= c0_cnt) {
								cur_oe = cur_oe->c[0];
								continue;
							}
							target_cnt -= c0_cnt;
							if (target_cnt == 1) {
								break;
							}
							target_cnt--;
							cur_oe = cur_oe->c[1];
							continue;
						}
						cur_oe->splay_no_update();
						nxt_oe->child_edges[!z] = cur_oe->c[1];
						if (cur_oe->c[1]) cur_oe->c[1]->p = nullptr;
						cur_oe->c[1] = nullptr;
						cur_oe->update();

						nxt_oe->c[0] = cur_oe;
						assert(cur_oe->p == nullptr);
						cur_oe->p = nxt_oe;
					}
				}

				nxt_oe->update();
				root->child_edges[!z] = nxt_oe;
			}

			assert(!cur_e->c[1]);
			assert(!cur_v->c[1]);

			auto splice = [&](auto& node_ptr, auto new_node) {
				auto cur = node_ptr;
				if (!cur) {
					node_ptr = new_node;
					return;
				}
				while (true) {
					cur->downdate();
					if (cur->c[1]) {
						cur = cur->c[1];
					} else {
						break;
					}
				}
				cur->splay_no_update();
				cur->c[1] = new_node;
				if (new_node) new_node->p = cur;
				cur->update();

				node_ptr = cur;
			};

			cerr << z << ' ' << cur_e->tot_sz << ' ' << x << '\n';
			assert(cur_e->tot_sz >= x);
			if (cur_e->tot_sz > x) {
				assert(cur_e->tot_sz - cur_e->own_sz < x);
				assert(cur_e == root->child_edges[z]);
				// Splice the new path
				root->child_edges[z] = cur_e->c[0];
				if (cur_e->c[0]) cur_e->c[0]->p = nullptr;
				cur_e->c[0] = nullptr;

				cur_v->spare = cur_e;
				cur_v->own_cnt[z] = 0;

				splice(root->child_edges[z], cur_e->child_edges[z]);
				splice(root->child_edges[!z], cur_e->child_edges[!z]);
				splice(root->child_vert, cur_e->child_vert);
				assert(root->child_vert == cur_v);

				*cur_e = edge_splay_node{};
			}

			root->update();
		}

		// flip and reverse the path
		//root->downdate(); // unnecessary
		assert(root->child_edges[0]->tot_sz == x);
		root->child_edges[0]->do_flip();
		root->child_edges[0]->do_reverse();
		return root->child_edges[0]->tot_cnt;
	};

	for (int q = 0; q < Q; q++) {
		int x; cin >> x;
		cerr << "op " << x << '\n';
		int res = op(x);
		cout << res << '\n';
	}

	return 0;
}
