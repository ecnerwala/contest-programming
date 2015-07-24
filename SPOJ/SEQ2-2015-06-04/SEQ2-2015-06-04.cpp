//start: 2015-06-04 08:00:15 AM
//only submit: 2015-06-04 08:35:39 AM
#include<bits/stdc++.h>
using namespace std;

struct node {
	node* p;
	node* c[2];

	inline int d() { return this == p->c[1]; }

	int val;
	bool lazy;
	bool flip;
	int sz;
	int sum;
	int msum;
	int lsum;
	int rsum;

	void update() {
		sz = 1 + (c[0] ? c[0]->sz : 0) + (c[1] ? c[1]->sz : 0);
		if(lazy) {
			sum = val * sz;
			lsum = rsum = max(0, sum);
			msum = max(val, sum);
		} else {
			sum = val + (c[0] ? c[0]->sum : 0) + (c[1] ? c[1]->sum : 0);
			lsum = max((c[0] ? c[0]->lsum : 0), (c[0] ? c[0]->sum : 0) + val + (c[1] ? c[1]->lsum : 0));
			rsum = max((c[1] ? c[1]->rsum : 0), (c[1] ? c[1]->sum : 0) + val + (c[0] ? c[0]->rsum : 0));
			msum = (c[0] ? c[0]->rsum : 0) + val + (c[1] ? c[1]->lsum : 0);
			if(c[0]) msum = max(msum, c[0]->msum);
			if(c[1]) msum = max(msum, c[1]->msum);
			if(flip) swap(lsum, rsum);
		}
	}

	void propogate() {
		if(flip) {
			swap(c[0], c[1]);
			if(c[0]) c[0]->flip ^= 1, swap(c[0]->lsum, c[0]->rsum);
			if(c[1]) c[1]->flip ^= 1, swap(c[1]->lsum, c[1]->rsum);
			flip = false;
		}
		if(lazy) {
			if(c[0]) c[0]->val = val, c[0]->lazy = true, c[0]->update();
			if(c[1]) c[1]->val = val, c[1]->lazy = true, c[1]->update();
			lazy = false;
		}
	}

	void rot() {
		int x = d();
		node* pa = p;
		node* ch = c[!x];
		if(pa->p) pa->p->c[pa->d()] = this;
		this->p = pa->p;

		pa->c[x] = ch;
		if(ch) ch->p = pa;

		c[!x] = pa;
		pa->p = this;

		pa->update();
	}

	void update_all(node* r) {
		for(node* cur = this; cur != r; cur = cur->p) {
			cur->update();
		}
	}

	node* splay(node* r) {
		propogate();
		while(p != r) {
			if(p->p != r) {
				if(p->d() == d()) p->rot();
				else rot();
			}
			rot();
		}
		update();
		return this;
	}

	node* get_next(int a) {
		propogate();
		if(!a) return this;
		node* n = c[1];
		while(true) {
			n->propogate();
			int s = n->c[0] ? n->c[0]->sz : 0;
			//cerr << n << ' ' << a << ' ' << s << '\n';
			if(a <= s) {
				n = n->c[0];
			} else if (a == s + 1) {
				break;
			} else {
				a -= s + 1;
				n = n->c[1];
			}
		}
		//cerr << n << '\n';
		n->splay(this);
		return n;
	}

	node* get_range(int a, int l) {
		assert(l);
		node* s = get_next(a - 1);
		if(s->c[1]->sz == l) return s->c[1];
		node* t = s->get_next(l + 1);
		return t->c[0];
	}

	// insert after the ath
	void insert(int a, node* n) {
		node* pa = this;
		while(true) {
			assert(pa);
			pa->propogate();
			int s = pa->c[0] ? pa->c[0]->sz : 0;
			if(a < s) {
				if(pa->c[0]) {
					pa = pa->c[0];
				} else {
					assert(a == -1);
					pa->c[0] = n;
					n->p = pa;
					break;
				}
			} else {
				a -= s + 1;
				if(pa->c[1]) {
					pa = pa->c[1];
				} else {
					assert(a == -1);
					pa->c[1] = n;
					n->p = pa;
					break;
				}
			}
		}
		n->splay(this);
		/*
		node* pa = get_next(a);
		if(!pa->c[1]) {
			n->p = pa;
			pa->c[1] = n;
		} else {
			pa = pa->c[1];
			while(pa->propogate(), pa->c[0]) {
				pa = pa->c[0];
			}
			n->p = pa;
			pa->c[0] = n;
		}
		n->splay(this);
		*/
	}

	node (int v, node* l, node* r) : p(NULL), c{l, r}, val(v), lazy(false), flip(false) {
		if(l) l->p = this;
		if(r) r->p = this;
		update();
	}

	~node() { delete c[0]; delete c[1]; }

	void print() {
		if(c[flip]) c[flip]->print();
		cerr << this << '\t' << val << '\t' << sz << '\n';
		if(c[!flip]) c[!flip]->print();
	}
};

int A[600000];

node* build_tree(int a, int b) {
	if(b < a) return NULL;
	int m = (a + b) / 2;
	return new node(A[m], build_tree(a, m - 1), build_tree(m + 1, b));
}
int main() {
	int T; scanf("%d", &T);
	node* R = new node(0, NULL, NULL);
	for(int t = 0; t < T; t++) {
		int N;
		int Q;
		scanf("%d %d", &N, &Q);

		for(int i = 0; i < N; i++) scanf("%d", &A[i]);
		R->c[1] = build_tree(0, N - 1);
		R->c[1]->p = R;

		for(int q = 0; q < Q; q++) {
			char c[20];
			scanf("%s", c);
			if(c[0] == 'I') {
				int a, l;
				scanf("%d %d", &a, &l);
				for(int i = 0; i < l; i++) scanf("%d", &A[i]);

				R->insert(a, build_tree(0, l - 1));
			} else if(c[0] == 'D') {
				int a, l;
				scanf("%d %d", &a, &l);
				if(!l) continue;
				node* n = R->get_range(a, l);
				n->p->c[n->d()] = NULL;
				if(n->p != R) n->p->splay(R);
				delete n;
			} else if(c[0] == 'R') {
				int a, l;
				scanf("%d %d", &a, &l);
				if(!l) continue;
				node* n = R->get_range(a, l);
				n->flip ^= 1;
				n->splay(R);
			} else if(c[0] == 'M' && c[2] == 'K') {
				int a, l, v;
				scanf("%d %d %d", &a, &l, &v);
				node* n = R->get_range(a, l);
				n->val = v;
				n->lazy = true;
				n->splay(R);
			} else if(c[0] == 'G') {
				int a, l;
				scanf("%d %d", &a, &l);
				if(!l) {printf("0\n"); continue;}
				node* n = R->get_range(a, l);
				printf("%d\n", n->sum);
			} else if(c[0] == 'M' && c[2] == 'X') {
				printf("%d\n", R->c[1]->msum);
			} else assert(false);
			//R->print();
			//fflush(stdout);
		}

		delete R->c[1];
		R->c[1] = NULL;
	}
	return 0;
}
