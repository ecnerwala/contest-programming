#include<bits/stdc++.h>
using namespace std;

struct node {
	node* p;
	node* c[2];

	inline int d() { return this == p->c[1]; }

	int sz;

	void update() {
		// TODO
	}
	void propogate() {
		// TODO
	}

	void rot() {
		int x = d();
		node* pa = p;
		node* ch = c[!x];

		if(pa->p) pa->p->c[pa->d()] = this;
		this->p = pa->p;

		pa->c[x] = ch;
		if(ch) ch->p = pa;

		this->c[!x] = pa;
		pa->p = this;

		pa->update();
	}

	void propogate_all(node* r) {
		if(p != r) propogate_all(p);
		propogate();
	}

	node* splay(node* r) {
		propogate_all(r);
		while(p != r) {
			if(p->p != r) {
				if(p->d() == this->d()) {
					p->rot();
				} else {
					rot();
				}
				rot();
			} else {
				rot();
			}
		}
		update();
		return this;
	}


	// find the ith in the subtree
	// 0-indexed
	node* find(int loc) {
		propogate();
		assert(0 <= loc);
		assert(loc < sz);
		int l = (c[0] ? c[0]->sz : 0);
		if(loc < l) {
			return c[0]->find(loc);
		} else if(loc == l) {
			return this;
		} else {
			return c[1]->find(loc - l - 1);
		}
	}

};

int main() {

}
