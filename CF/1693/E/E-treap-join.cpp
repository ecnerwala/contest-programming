#include <bits/stdc++.h>

std::mt19937 mt(48);
struct treap_node {
	std::mt19937::result_type pri = mt();
	std::array<treap_node*, 2> c = {nullptr, nullptr};
	int val;
	int sz = 1;

	void update() {
		sz = 1;
		if (c[0]) sz += c[0]->sz;
		if (c[1]) sz += c[1]->sz;
	}
};

std::pair<treap_node*, treap_node*> split(treap_node* a, int v) {
	if (!a) return {nullptr, nullptr};
	treap_node *x, *y;
	if (a->val < v) {
		x = a;
		std::tie(x->c[1], y) = split(a->c[1], v);
	} else {
		y = a;
		std::tie(x, y->c[0]) = split(a->c[0], v);
	}
	a->update();
	return {x, y};
}

treap_node* treap_join(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	treap_node *r;
	if (a->pri < b->pri) {
		r = a;
		r->c[1] = treap_join(a->c[1], b);
	} else {
		r = b;
		r->c[0] = treap_join(a, b->c[0]);
	}
	r->update();
	return r;
}

treap_node* treap_union(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	if (a->pri > b->pri) std::swap(a, b);
	auto [x, y] = split(b, a->val);
	a->c[0] = treap_union(a->c[0], x);
	a->c[1] = treap_union(a->c[1], y);
	a->update();
	return a;
}

void dump_treap(treap_node* v) {
	if (!v) return;
	dump_treap(v->c[0]);
	std::cerr << v->val << ' ';
	dump_treap(v->c[1]);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	N += 2;
	std::vector<int> A(N);
	A[0] = 0;
	for (int i = 1; i+1 < N; i++) {
		cin >> A[i];
	}
	A[N-1] = 0;

	std::vector<treap_node> nodes(N);
	for (int i = 0; i < N; i++) {
		nodes[i].val = i;
		nodes[i].update();
	}
	const int V = N;
	std::vector<std::vector<int>> idxs(V+1);
	for (int i = 0; i < N; i++) {
		idxs[A[i]].push_back(i);
	}

	treap_node* ls = nullptr;
	treap_node* cens = nullptr;
	treap_node* rs = nullptr;

	int64_t ans = 0;
	for (int v = V; v > 0; v--) {
		if (idxs[v].empty()) continue;
		int lo = idxs[v].front();
		int hi = idxs[v].back();

		treap_node* nl = nullptr;
		treap_node* nc = nullptr;
		treap_node* nr = nullptr;

		{
			auto [x, y] = split(ls, lo);
			nl = treap_join(nl, x);
			nc = treap_join(nc, y);
		}
		{
			auto [l, mr] = split(cens, lo);
			auto [m, r] = split(mr, hi);
			nl = treap_join(nl, l);
			nc = treap_join(nc, m);
			nr = treap_join(nr, r);
		}

		{
			auto [x, y] = split(rs, hi);
			nc = treap_join(nc, x);
			nr = treap_join(nr, y);
		}

		treap_node* curs = nullptr;
		for (int a : idxs[v]) {
			curs = treap_join(curs, &nodes[a]);
		}
		nc = treap_union(nc, curs);

		ls = nl;
		cens = nc;
		rs = nr;

		ans += cens ? cens->sz : 0;
		/*
		cerr << v << ' ' << '\n';
		cerr << "left" << ' '; dump_treap(ls); cerr << '\n';
		cerr << "mid" << ' '; dump_treap(cens); cerr << '\n';
		cerr << "right" << ' '; dump_treap(rs); cerr << '\n';
		*/
	}
	cout << ans << '\n';

	return 0;
}
