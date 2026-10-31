#include <bits/stdc++.h>

std::mt19937 mt(48);
struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	int64_t val = 0;
	int64_t lazy = 0;
	int sz = 1;
	void apply_lazy(int64_t l) {
		val += l;
		lazy += l;
	}
	void downdate() {
		if (lazy) {
			if (c[0]) c[0]->apply_lazy(lazy);
			if (c[1]) c[1]->apply_lazy(lazy);
		}
		lazy = 0;
	}
	void update() {
		sz = 1 + (c[0] ? c[0]->sz : 0) + (c[1] ? c[1]->sz : 0);
	}
};

int64_t get_last(treap_node* a) {
	assert(a);
	while (a->c[1]) {
		a->downdate();
		a = a->c[1];
	}
	return a->val;
}

int64_t get_first(treap_node* a) {
	assert(a);
	while (a->c[0]) {
		a->downdate();
		a = a->c[0];
	}
	return a->val;
}

treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	treap_node* r;
	if (a->pri < b->pri) {
		r = a;
		r->downdate();
		r->c[1] = merge(a->c[1], b);
		r->update();
	} else {
		r = b;
		r->downdate();
		r->c[0] = merge(a, b->c[0]);
		r->update();
	}
	return r;
}

std::pair<treap_node*, treap_node*> split_by_sz(treap_node* a, int lsz) {
	if (!a) {
		assert(lsz == 0);
		return {nullptr, nullptr};
	}
	assert(0 <= lsz && lsz <= a->sz);
	if (lsz == 0) return {nullptr, a};
	if (lsz == a->sz) return {a, nullptr};
	a->downdate();
	int c0sz = (a->c[0] ? a->c[0]->sz : 0);
	treap_node *x, *y;
	if (lsz <= c0sz) {
		y = a;
		std::tie(x, y->c[0]) = split_by_sz(a->c[0], lsz);
	} else {
		x = a;
		std::tie(x->c[1], y) = split_by_sz(a->c[1], lsz - c0sz - 1);
	}
	a->update();
	return {x, y};
}

// split into <= and >
std::pair<treap_node*, treap_node*> split_by_pos(treap_node* a, int pos) {
	if (!a) {
		return {nullptr, nullptr};
	}
	a->downdate();
	treap_node *x, *y;
	if (a->val > pos) {
		y = a;
		std::tie(x, y->c[0]) = split_by_pos(a->c[0], pos);
	} else {
		x = a;
		std::tie(x->c[1], y) = split_by_pos(a->c[1], pos);
	}
	a->update();
	return {x, y};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t D; int Q; cin >> N >> D >> Q; D--;
	std::vector<int64_t> A_init(N);
	for (auto& a : A_init) cin >> a;
	std::string is_movable; cin >> is_movable;

	struct query_t {
		int64_t t;
		int idx;
		int q;
	};
	std::vector<query_t> queries(Q);
	for (auto& q : queries) {
		cin >> q.t >> q.idx; q.idx--;
	}
	for (int q = 0; q < Q; q++) queries[q].q = q;
	std::sort(queries.begin(), queries.end(), [&](auto a, auto b) -> bool {
		return a.t < b.t;
	});

	std::vector<int64_t> immovables; immovables.reserve(N);
	std::vector<int64_t> movables; movables.reserve(N);
	for (int i = 0; i < N; i++) {
		int64_t pos = A_init[i] - int(immovables.size());
		if (is_movable[i] == '1') {
			movables.push_back(pos);
		} else {
			immovables.push_back(pos);
		}
	}

	std::vector<treap_node> nodes(movables.size());
	for (int i = 0; i < int(movables.size()); i++) {
		nodes[i].val = movables[i];
		nodes[i].lazy = 0;
		nodes[i].update();
	}

	treap_node* root = &nodes[0];
	treap_node* oroot = nullptr;
	for (int i = 1; i < int(movables.size()); i++) {
		oroot = merge(oroot, &nodes[i]);
	}
	int mcnt = 1;

	auto query = [&](int idx) -> int64_t {
		// need to query immovables, movables suffix, and root
		assert(mcnt == root->sz);
		root = merge(root, oroot);
		treap_node* cur = root;
		while (true) {
			if (!cur) {
				int64_t ans = immovables[idx] + idx;
				std::tie(root, oroot) = split_by_sz(root, mcnt);
				return ans;
			}
			cur->downdate();
			int lcnt = (cur->c[0] ? cur->c[0]->sz : 0);
			int num_immo = int(std::upper_bound(immovables.begin(), immovables.end(), cur->val) - immovables.begin());
			lcnt += num_immo;
			if (lcnt == idx) {
				int64_t ans = cur->val + num_immo;
				std::tie(root, oroot) = split_by_sz(root, mcnt);
				return ans;
			} else if (idx < lcnt) {
				cur = cur->c[0];
			} else if (idx > lcnt) {
				idx -= (cur->c[0] ? cur->c[0]->sz : 0);
				idx --;
				cur = cur->c[1];
			}
		}
		assert(false);
	};

	std::vector<int64_t> ans(Q);

	int64_t cur_time = 0;
	for (auto q : queries) {
		while (cur_time < q.t) {
			int64_t delta_t = q.t - cur_time;
			// coords += d + sz every time
			assert(mcnt == root->sz);
			if (mcnt < int(movables.size())) {
				int64_t hi = get_last(root);
				assert(movables[mcnt] > hi);
				int64_t max_full = (movables[mcnt] - 1 - hi) / (D + mcnt);
				if (max_full > 0 && delta_t / mcnt > 0) {
					int64_t num_cycs = std::min(max_full, delta_t / mcnt);
					root->apply_lazy(num_cycs * (D + mcnt));
					cur_time += num_cycs * mcnt;
					continue;
				} else if (max_full > 0) {
					assert(delta_t < mcnt);
					// cycle just part
					auto [x, y] = split_by_sz(root, int(delta_t));
					assert(x);
					x->apply_lazy(D + mcnt);
					root = merge(y, x);
					cur_time += delta_t;
					continue;
				} else {
					assert(max_full == 0);
					auto [x, y] = split_by_pos(root, movables[mcnt] - 1 - (D + mcnt));
					if (!x) {
						auto [z, noroot] = split_by_sz(oroot, 1);
						oroot = noroot;
						assert(z == &nodes[mcnt]);
						root = merge(y, z);
						mcnt++;
						continue;
					}
					// otherwise, we can cycle up to x forwards
					assert(x);
					if (x->sz <= delta_t) {
						// cycle this many forwards
						x->apply_lazy(D + mcnt);
						cur_time += x->sz;
						root = merge(y, x);
						continue;
					} else {
						root = merge(x, y);
						{
							auto [x, y] = split_by_sz(root, int(delta_t));
							assert(x);
							x->apply_lazy(D + mcnt);
							root = merge(y, x);
							cur_time += delta_t;
							continue;
						}
					}
				}
				assert(false);
			} else {
				// just cycle forwards
				root->apply_lazy((delta_t / mcnt) * (D + mcnt));
				delta_t %= mcnt;
				assert(delta_t < mcnt);
				if (delta_t) {
					auto [x, y] = split_by_sz(root, int(delta_t));
					assert(x);
					x->apply_lazy(D + mcnt);
					root = merge(y, x);
				}
				cur_time = q.t;
			}
		}
		assert(cur_time == q.t);
		ans[q.q] = query(q.idx);
	}

	for (auto v : ans) cout << v << '\n';

	return 0;
}
