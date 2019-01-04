#include<bits/stdc++.h>
using namespace std;

mt19937_64 rng = mt19937_64(1234);

struct node {
	node* c[2];
	uint64_t priority;

	char value;

	bool flip;

	int len;
	int max_mid;
	int max_pref;
	int max_suff;

	node (char chr) {
		c[0] = c[1] = nullptr;
		value = chr;
		priority = rng();
		flip = false;
		update();
	}

	~node() {
		delete c[0];
		delete c[1];
	}

	void update() {
		downdate();
		assert(!flip);
		len = 1 + (c[0] ? c[0]->len : 0) + (c[1] ? c[1]->len : 0);
		max_pref = (c[0] ? c[0]->max_pref : 0);
		max_suff = (c[1] ? c[1]->max_suff : 0);
		max_mid = max((c[0] ? c[0]->max_mid : 0), (c[1] ? c[1]->max_mid : 0));
		if (value == '1') {
			if (max_pref == (c[0] ? c[0]->len : 0)) {
				max_pref += 1 + (c[1] ? c[1]->max_pref : 0);
			}
			if (max_suff == (c[1] ? c[1]->len : 0)) {
				max_suff += 1 + (c[0] ? c[0]->max_suff : 0);
			}
			max_mid = max(max_mid, (c[0] ? c[0]->max_suff : 0) + 1 + (c[1] ? c[1]->max_pref : 0));
		}
	}

	void downdate() {
		if (flip) {
			swap(c[0], c[1]);
			if(c[0]) c[0]->do_flip();
			if(c[1]) c[1]->do_flip();
			flip = false;
		}
		assert(!flip);
	}

	void do_flip() {
		flip = !flip;
		swap(max_pref, max_suff);
	}
};

void merge(node* &t, node* l, node* r) {
	if (!l) {
		t = r;
		return;
	}
	if (!r) {
		t = l;
		return;
	}

	l->downdate();
	r->downdate();

	if (l->priority < r->priority) {
		t = l;
		merge(l->c[1], l->c[1], r);
	} else {
		t = r;
		merge(r->c[0], l, r->c[0]);
	}
	t->update();
}

void split(node* n, node* &l, node* &r, int k) {
	if (!n) {
		assert(k == 0);
		l = r = nullptr;
		return;
	}

	assert(0 <= k && k <= n->len);
	n->downdate();

	int llen = n->c[0] ? n->c[0]->len : 0;
	if (k <= llen) {
		r = n;
		split(n->c[0], l, n->c[0], k);
	} else {
		l = n;
		split(n->c[1], n->c[1], r, k - llen - 1);
	}
	n->update();
}

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	int N, Q; cin >> N >> Q;

	node* root = nullptr;
	string S; cin >> S;
	for (char c : S) {
		merge(root, root, new node(c));
	}

	for (int q = 0; q < Q; q++) {
		int t, i, l; cin >> t >> i >> l;
		node* pref;
		node* mid;
		node* suff;
		split(root, pref, root, i);
		split(root, mid, suff, l);
		if (t == 1) {
			mid->do_flip();
		} else {
			cout << mid->max_mid << '\n';
		}
		merge(root, mid, suff);
		merge(root, pref, root);
	}

	delete root;

	return 0;
}
