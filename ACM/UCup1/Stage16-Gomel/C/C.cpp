#include <bits/stdc++.h>

std::mt19937_64 mt(48);
struct treap_node {
	std::array<treap_node*, 2> c = {nullptr, nullptr};

	std::mt19937_64::result_type pri = mt();
	std::pair<uint32_t, int> key;

	uint32_t c0, c1;
	uint32_t tot_c0, tot_c1;
	void update() {
		tot_c0 = c0 + (c[0] ? c[0]->tot_c0 : 0) + (c[1] ? c[1]->tot_c0 : 0);
		tot_c1 = c1 + (c[0] ? c[0]->tot_c1 : 0) + (c[1] ? c[1]->tot_c1 : 0);
	}
};

treap_node* merge(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
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

std::pair<treap_node*, treap_node*> split(treap_node* r, std::pair<uint32_t, int> key) {
	if (!r) return {nullptr, nullptr};
	treap_node* a;
	treap_node* b;
	if (r->key < key) {
		a = r;
		std::tie(a->c[1], b) = split(r->c[1], key);
	} else {
		b = r;
		std::tie(a, b->c[0]) = split(r->c[0], key);
	}
	r->update();
	return {a, b};
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int M; cin >> M;

	const int L = (1 << M);

	std::vector<treap_node> nodes(2*N);

	treap_node* root = nullptr;

	uint32_t x = 0;
	for (int iter = 0; iter < N; iter++) {
		//cerr << "iter " << iter << '\n';
		uint32_t p, q; cin >> p >> q;
		//cerr << "input " << p << ' ' << q << '\n';
		p += uint32_t(x), q += uint32_t(x);
		p &= (L-1);
		q &= (L-1);
		if (p > q) std::swap(p, q);
		assert(p <= q);
		q++;
		//cerr << "actual " << p << ' ' << q << '\n';

		uint32_t new_val = uint32_t(iter+1);

		treap_node* n0 = &nodes[2*iter+0];
		n0->key = {p, 2*iter+0};
		n0->c0 = -p * new_val;
		n0->c1 = new_val;
		n0->update();

		treap_node* n1 = &nodes[2*iter+1];
		n1->key = {q, 2*iter+1};
		n1->c0 = q * new_val;
		n1->c1 = -new_val;
		n1->update();

		treap_node *lo, *mi, *hi;
		std::tie(lo, mi) = split(root, n0->key);
		std::tie(mi, hi) = split(mi, n1->key);

		lo = merge(lo, n0);
		mi = merge(mi, n1);

		uint32_t vq = q * (lo->tot_c1 + mi->tot_c1) + (lo->tot_c0 + mi->tot_c0);
		uint32_t vp = p * (lo->tot_c1) + (lo->tot_c0);
		x += vq - vp;
		//cerr << "x = " << x << '\n';

		root = merge(merge(lo, mi), hi);
	}

	x &= (1 << 30) - 1;
	cout << x << '\n';

	return 0;
}
