#include <bits/stdc++.h>

std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
struct treap_node {
	std::mt19937_64::result_type pri;
	std::array<treap_node*, 2> c;
	int val;
};

std::pair<treap_node*, treap_node*> split(treap_node* a, int v) {
	if (!a) return {a, a};
	treap_node *l, *r;
	if (v < a->val) {
		r = new treap_node(*a);
		std::tie(l, r->c[0]) = split(a->c[0], v);
	} else if (v > a->val) {
		l = new treap_node(*a);
		std::tie(l->c[1], r) = split(a->c[1], v);
	} else assert(false);
	return {l, r};
}

treap_node* insert(treap_node* r, int v, std::mt19937_64::result_type pri) {
	if (r && r->pri < pri) {
		assert(r->val != v);
		r = new treap_node(*r);
		if (v < r->val) {
			r->c[0] = insert(r->c[0], v, pri);
		} else if (v > r->val) {
			r->c[1] = insert(r->c[1], v, pri);
		} else assert(false);
		return r;
	} else {
		treap_node* n = new treap_node;
		n->pri = pri;
		n->val = v;
		std::tie(n->c[0], n->c[1]) = split(r, v);
		return n;
	}
}

bool contain(treap_node* r, int v) {
	while (true) {
		if (!r) return false;
		if (v > r->val) {
			r = r->c[1];
		} else if (v < r->val) {
			r = r->c[0];
		} else if (v == r->val) {
			return true;
		} else assert(false);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	int N = 1;
	std::vector<int> par; par.reserve(Q+1);
	std::vector<int> jump_par; jump_par.reserve(Q+1);
	std::vector<int> jump_dist; jump_dist.reserve(Q+1);
	std::vector<treap_node*> roots;
	std::vector<int> sizes;

	par.push_back(-1);
	jump_par.push_back(0);
	jump_dist.push_back(0);
	roots.push_back(nullptr);
	sizes.push_back(0);

	std::vector<int> stk; stk.reserve(Q+1);
	stk.push_back(0);

	for (int q = 0; q < Q; q++) {
		char op; cin >> op;
		if (op == '+') {
			int a; cin >> a;
			int cur = stk.back();
			int nxt = N++;
			par.push_back(cur);
			if (jump_dist[cur] == jump_dist[jump_par[cur]]) {
				jump_par.push_back(jump_par[jump_par[cur]]);
				jump_dist.push_back(2 * jump_dist[cur] + 1);
			} else {
				jump_par.push_back(cur);
				jump_dist.push_back(1);
			}
			auto n = roots[cur];
			int sz = sizes[cur];
			if (!contain(n, a)) {
				n = insert(n, a, rng());
				sz++;
			}
			roots.push_back(n);
			sizes.push_back(sz);

			stk.push_back(nxt);
		} else if (op == '-') {
			int k; cin >> k;
			int cur = stk.back();
			while (k > 0) {
				assert(cur != 0);
				if (k >= jump_dist[cur]) {
					k -= jump_dist[cur];
					cur = jump_par[cur];
				} else {
					k--;
					cur = par[cur];
				}
			}
			stk.push_back(cur);
		} else if (op == '!') {
			assert(stk.size() >= 2);
			stk.pop_back();
		} else if (op == '?') {
			cout << sizes[stk.back()] << '\n' << flush;
		} else assert(false);
	}

	return 0;
}
