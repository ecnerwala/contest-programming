// start at 2015-06-03 08:51:00 PM
// first submit 2015-06-03 10:02:14 PM
// finish at 2015-06-03 10:22:20 PM
#define NDEBUG
#include<bits/stdc++.h>
using namespace std;

struct node {
	node* p;
	node* c[2];

	// value
	int val;

	bool lazy;
	bool flip;

	int sz;

	int sum;
	int lsum;
	int rsum;
	int msum;

	inline int d() { return this == p->c[1]; }

	void update() {
		//cerr << "updating" << ' ' << this << '\n';
		sz = 1 + (c[0] ? c[0]->sz : 0) + (c[1] ? c[1]->sz : 0);

		if(lazy) {
			sum = val * sz;
			lsum = rsum = max(0, sum);
			msum = max(val, sum);
		} else {
			sum = val + (c[0] ? c[0]->sum : 0) + (c[1] ? c[1]->sum : 0);

			msum = max((c[0] ? c[0]->rsum : 0) + val + (c[1] ? c[1]->lsum : 0), max(c[0] ? c[0]->msum : val, c[1] ? c[1]->msum : val));

			lsum = max((c[0] ? c[0]->lsum : 0), (c[0] ? c[0]->sum : 0) + val + (c[1] ? c[1]->lsum : 0));
			rsum = max((c[1] ? c[1]->rsum : 0), (c[1] ? c[1]->sum : 0) + val + (c[0] ? c[0]->rsum : 0));

			if(flip) {
				swap(lsum, rsum);
			}
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
		assert(p);

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
	/*
	void propogate_all(node* r) {
		if(p != r) p->propogate_all(r);
		propogate();
	}
	*/

	node* splay(node* r) {
		//propogate_all(r);
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

	// find in subtree
	// 0 indexed
	node* find(int a) {
		assert(0 <= a);
		assert(a < sz);

		propogate();

		assert(!flip);
		int s = (c[0] ? c[0]->sz : 0);
		if(a < s) {
			return c[0]->find(a);
		} else if(a == s) {
			return this;
		} else {
			return c[1]->find(a - s - 1);
		}
	}

	// get the ith after this
	node* get_next(int a) {
		propogate();
		assert(!flip);
		if(a == 0) return this;
		else return c[1]->find(a - 1)->splay(this);
	}

	// get the range from a to b, inclusive
	// 1-indexed, starts after the current one
	node* get_range(int a, int l) {
		assert(l > 0);
		assert(1 <= a && a + l - 1 <= c[1]->sz);

		node* s = get_next(a - 1);

		assert(!s->flip);
		assert(s->c[1]);
		if(s->c[1]->sz == l) {
			return s->c[1];
		} else {
			node* t = s->get_next(l + 1);
			assert(t->c[0]->sz == l);
			return t->c[0];
		}
	}

	// insert after get_next(a)
	void insert(int a, node *n) {
		node* v = get_next(a);
		assert(!v->flip);
		if(!v->c[1]) {
			v->c[1] = n;
			n->p = v;
		} else {
			node* pa = v->c[1];
			while(pa->propogate(), pa->c[0]) {
				pa = pa->c[0];
			}
			n->p = pa;
			pa->c[0] = n;
		}
		n->splay(this);
	}

	~node() {
		delete c[0];
		delete c[1];
	}

	node(int v, node* l = NULL, node* r = NULL) : p(NULL), c{l,r}, val(v), lazy(false), flip(false) {
		if(l) l->p = this;
		if(r) r->p = this;
		update();
	}

	void print() {
		if(c[flip]) c[flip]->print();
		//cerr << this << '\t' << val << '\t' << lazy << '\t' << flip << '\t' << sz << '\t' << sum << '\t' << lsum << '\t' << rsum << '\t' << msum << '\n';
		if(c[!flip]) c[!flip]->print();
	}

};

int S;
int A[600000];

// build the tree from [a, b]
node* build_tree(int a, int b) {
	if(b < a) return NULL;
	int m = (a + b) / 2;
	assert(a <= m && m <= b);
	return new node(A[m], build_tree(a, m - 1), build_tree(m + 1, b));
}

char op[20];

int main() {
	int T; scanf("%d", &T);

	for(int t = 0; t < T; t++) {
		node* ROOT = new node(0);

		int N, M;
		scanf("%d %d", &N, &M);

		for(int q = 0; q <= M; q++) {
			if(q) {
				scanf("%s", op);
			} else {
				op[0] = 'I';
			}

			if(op[0] == 'I') {
				int i;
				if(q) {
					scanf("%d %d", &i, &S);
				} else {
					i = 0;
					S = N;
				}

				for(int s = 0; s < S; s++) {
					scanf("%d", &A[s]);
				}

				ROOT->insert(i, build_tree(0, S - 1));
			} else if(op[0] == 'D') {
				int a, l;
				scanf("%d %d", &a, &l);
				if(!l) continue;
				node* n = ROOT->get_range(a, l);
				node* p = n->p;
				p->c[n->d()] = NULL;
				n->update_all(ROOT);
				delete n;
			} else if(op[0] == 'R') {
				int a, l;
				scanf("%d %d", &a, &l);
				if(!l) continue;
				node* n = ROOT->get_range(a, l);

				n->flip ^= 1;

				n->update_all(ROOT);
			} else if(op[0] == 'G') {
				int a, l;
				scanf("%d %d", &a, &l);
				if(!l) {
					printf("0\n");
					continue;
				}
				node* n = ROOT->get_range(a, l);

				printf("%d\n", n->sum);
			} else if(op[0] == 'M' && op[2] == 'K') {
				int a, l, v;
				scanf("%d %d %d", &a, &l, &v);
				if(!l) continue;
				node* n = ROOT->get_range(a, l);

				n->val = v;
				n->lazy = true;

				n->update_all(ROOT);
			} else if(op[0] == 'M' && op[2] == 'X') {
				printf("%d\n", ROOT->c[1]->msum);
			} else assert(false);

			//fflush(stdout);
			//cerr << "this\t" << '\t' << "val" << '\t' << "lazy" << '\t' << "flip" << '\t' << "sz" << '\t' << "sum" << '\t' << "lsum" << '\t' << "rsum" << '\t' << "msum" << '\n';
			//ROOT->c[1]->print();
		}

		delete ROOT;
	}

	return 0;
}
