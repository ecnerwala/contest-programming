#include <bits/stdc++.h>

struct lin_func {
	int64_t slope = 1, cons = 0;

	// apply a then apply b
	lin_func& operator *= (lin_func b) {
		slope *= b.slope;
		cons *= b.slope;
		cons += b.cons;
		return *this;
	}
	friend lin_func operator * (lin_func a, lin_func b) {
		return lin_func(a) *= b;
	}

	int64_t operator() (int64_t a) const {
		return a * slope + cons;
	}
};

std::mt19937_64 mt(480);

struct treap_node {
	std::array<treap_node*, 2> c;
	std::mt19937_64::result_type pri = mt();
	std::pair<int64_t, int> val;
	lin_func lazy;

	void apply_lazy(lin_func o) {
		val.first = o(val.first);
		lazy *= o;
	}

	void downdate() {
		for (treap_node* n : c) {
			if (n) n->apply_lazy(lazy);
		}
		lazy = lin_func{};
	}

	void downdate_tree() {
		downdate();
		for (treap_node* n : c) if (n) n->downdate_tree();
	}
};

std::pair<treap_node*, treap_node*> split(treap_node* r, std::pair<int64_t, int> key) {
	if (!r) return {nullptr, nullptr};
	r->downdate();
	treap_node *a, *b;
	if (r->val < key) {
		a = r;
		std::tie(a->c[1], b) = split(r->c[1], key);
	} else {
		b = r;
		std::tie(a, b->c[0]) = split(r->c[0], key);
	}
	return {a, b};
}

treap_node* merge(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	treap_node *r;
	if (a->pri < b->pri) {
		a->downdate();
		r = a;
		r->c[1] = merge(a->c[1], b);
	} else {
		b->downdate();
		r = b;
		r->c[0] = merge(a, b->c[0]);
	}
	return r;
}

treap_node* meld(treap_node* a, treap_node* b) {
	if (!b) return a;
	if (!a) return b;
	if (a->pri > b->pri) std::swap(a, b);
	a->downdate();
	auto [x, y] = split(b, a->val);
	a->c[0] = meld(a->c[0], x);
	a->c[1] = meld(a->c[1], y);
	return a;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	N++;
	std::vector<int> P(N);
	std::vector<int64_t> M(N);
	P[0] = -1, M[0] = 0;
	for (int i = 1; i < N; i++) {
		cin >> P[i] >> M[i];
	}
	std::vector<treap_node> nodes(N);
	for (int i = 0; i < N; i++) nodes[i].val.second = i;

	std::vector<treap_node*> roots(N, nullptr);

	std::vector<std::vector<int64_t>> ch(N);
	for (int i = N-1; i >= 0; i--) {
		nodes[i].val.first = M[i];

		treap_node* nroot = nullptr;

		int L = int(ch[i].size());
		std::sort(ch[i].begin(), ch[i].end());
		int64_t cons = 0;
		for (int z = 0; z < L; z++) {
			treap_node* cur;
			std::tie(cur, roots[i]) = split(roots[i], {ch[i][z], N});

			if (cur) cur->apply_lazy(lin_func{L-z, cons});
			nroot = merge(nroot, cur);

			cons += ch[i][z];
		}

		assert(roots[i] == nullptr);
		roots[i] = merge(nroot, &nodes[i]);

		if (i > 0) {
			int p = P[i];
			M[p] += M[i];
			ch[p].push_back(M[i]);
			roots[p] = meld(roots[p], roots[i]);
		}
	}

	roots[0]->downdate_tree();
	for (int i = 1; i < N; i++) {
		cout << nodes[i].val.first << '\n';
	}

	return 0;
}
