#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e5;
const int MAXQ = 2e5;

int N;

int F[MAXN];

pair<int, pii> E[MAXN]; // edges

int Q;

pair<int, pii> V[MAXQ]; //queries

struct node {
	node *p;
	node *c[2];

	bool flip;

	inline bool hasp() {
		return p && (this == p->c[0] || this == p->c[1]);
	}

	inline int d() {
		return this == p->c[1];
	}

	int ind;

	int lval, rval, mval;

	void update() {
		if(ind < N) {
		}
	}

	void propogate() {
	}

	void propall() {
		while(hasp()) {
			p->propall();
		}
		propogate();
	}

	void rot() {
		int x = d();
		node* pa = p;
		node* ch = c[!x];

		if(pa->p) pa->p->c[p->d()] = this;
		this->p = pa->p;

		pa->c[x] = ch;
		if(ch) ch->p = pa;

		c[!x] = pa;
		pa->p = this;
	}

	void splay() {
		while(hasp()) {
			if(p->hasp()) {
				if(p->d() == d()) p->rot();
				else rot();
			}
			rot();
		}
	}
};

int main() {

	return 0;
}
