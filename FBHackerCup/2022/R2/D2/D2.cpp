#include<bits/stdc++.h>

std::mt19937 mt(48);
struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	int key = -1;
	int sz = 1;
	int64_t sum = -1;

	void update() {
		sz = 1 + (c[0] ? c[0]->sz : 0) + (c[1] ? c[1]->sz : 0);
		sum = key + (c[0] ? c[0]->sum : 0) + (c[1] ? c[1]->sum : 0);
	}
};

treap_node* merge(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	treap_node* r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	r->update();
	return r;
}

// split into < key and >= key
std::pair<treap_node*, treap_node*> split_by_key(treap_node* a, int key) {
	if (!a) return {nullptr, nullptr};
	treap_node *x, *y;
	if (key <= a->key) {
		y = a;
		std::tie(x, y->c[0]) = split_by_key(a->c[0], key);
	} else {
		x = a;
		std::tie(x->c[1], y) = split_by_key(a->c[1], key);
	}
	a->update();
	return {x, y};
}

std::pair<treap_node*, treap_node*> split_by_size(treap_node* a, int l_sz) {
	assert(0 <= l_sz && l_sz <= (a ? a->sz : 0));
	if (!a) return {nullptr, nullptr};
	assert(a);
	treap_node *x, *y;
	if (l_sz <= (a->c[0] ? a->c[0]->sz : 0)) {
		y = a;
		std::tie(x, y->c[0]) = split_by_size(a->c[0], l_sz);
	} else {
		x = a;
		std::tie(x->c[1], y) = split_by_size(a->c[1], l_sz - 1 - (a->c[0] ? a->c[0]->sz : 0));
	}
	a->update();
	return {x, y};
}

treap_node* insert(treap_node* a, treap_node* n) {
	auto [x, y] = split_by_key(a, n->key);
	return merge(merge(x, n), y);
}

treap_node* erase(treap_node* a, treap_node* n) {
	auto [x, y] = split_by_key(a, n->key);
	auto [m, z] = split_by_key(y, n->key + 1);
	assert(m == n);
	assert(m->sz == 1);
	return merge(x, z);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, Q; cin >> N >> Q;
		cerr << "Case #" << case_num << " " << "N = " << N << " Q = " << Q << '\n';
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::array<treap_node*, 3> roots{nullptr, nullptr, nullptr};

		std::vector<treap_node> nodes(N);
		for (int i = 0; i < N; i++) {
			nodes[i].key = i;
			nodes[i].update();
			roots[A[i]] = insert(roots[A[i]], &nodes[i]);
		}

		int64_t tot_ans = 0;
		for (int q = 0; q < Q; q++) {
			{
				int i, y; cin >> i >> y;
				i --;
				//cerr << "Update " << i << ' ' << y << '\n';
				roots[A[i]] = erase(roots[A[i]], &nodes[i]);
				A[i] = y;
				roots[A[i]] = insert(roots[A[i]], &nodes[i]);
			}

			{
				int z; cin >> z;
				//cerr << "Query " << z << '\n';
				auto [l1, r1] = split_by_key(roots[1], z);
				auto [l2, r2] = split_by_key(roots[2], z);

				auto move_cost = [&](treap_node* &l, treap_node* &r, int num_move) -> int64_t {
					assert(num_move >= 0);
					if ((l ? l->sz : 0) < num_move) return -1;
					if ((r ? r->sz : 0) < num_move) return -1;
					int64_t l_idx;
					{
						auto [x, y] = split_by_size(l, (l ? l->sz : 0) - num_move);
						l_idx = y ? y->sum : 0;
						auto nl = merge(x, y);
						assert(l == nl);
					}
					int64_t r_idx;
					{
						auto [x, y] = split_by_size(r, num_move);
						r_idx = x ? x->sum : 0;
						auto nr = merge(x, y);
						assert(r == nr);
					}
					return r_idx - l_idx;
				};

				int64_t ans = [&]() -> int64_t {
					int diff =
						(l1 ? l1->sz : 0) + (l2 ? l2->sz : 0) * 2
					- (r1 ? r1->sz : 0) - (r2 ? r2->sz : 0) * 2;

					if (diff & 1) return -1;
					diff /= 2;

					if (diff > 0) {
						// l2 moves to r1
						return move_cost(l2, r1, diff);
					} else {
						// l1 moves to r2
						return move_cost(l1, r2, -diff);
					}
				}();

				auto n1 = merge(l1, r1);
				assert(roots[1] == n1);
				auto n2 = merge(l2, r2);
				assert(roots[2] == n2);
				//cerr << ans << '\n';
				tot_ans += ans;
			}
		}

		cout << "Case #" << case_num << ": " << tot_ans << '\n';
	}

	return 0;
}
