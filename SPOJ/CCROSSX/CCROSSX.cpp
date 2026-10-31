#include<bits/stdc++.h>
using namespace std;

using ll = long long;

struct node {
	node* p = nullptr;
	node* c[2] = {nullptr, nullptr};

	int d() { return this == p->c[1]; }

	ll segLen;
	ll segSlope;

	ll lazySlope;

	ll rangeLen;
	ll rangeDelta;

	void update() {
		rangeLen = (c[0] ? c[0]->rangeLen : 0) + segLen + (c[1] ? c[1]->rangeLen : 0);
		rangeDelta = (c[0] ? c[0]->rangeDelta : 0) + segLen * segSlope + (c[1] ? c[1]->rangeDelta : 0);
		rangeDelta += lazySlope * rangeLen;
	}

	void addSlope(ll s) {
		lazySlope += s;
		rangeDelta += s * rangeLen;
	}

	void downdate() {
		if (lazySlope) {
			segSlope += lazySlope;
			if (c[0]) c[0]->addSlope(lazySlope);
			if (c[1]) c[1]->addSlope(lazySlope);
			lazySlope = 0;
		}
	}

	void rot() {
		assert(p);

		int x = d();
		node* pa = p;
		node* ch = c[!x];

		this->p = pa->p;
		if (pa->p) pa->p->c[pa->d()] = this;

		pa->c[x] = ch;
		if (ch) ch->p = pa;

		this->c[!x] = pa;
		pa->p = this;

		pa->update();
	}

	void downdateAll(node* root = nullptr) {
		if (p != root) p->downdateAll(root);
		downdate();
	}

	void splay(node* root = nullptr) {
		downdateAll(root);
		while (p != root) {
			if (p->p != root) {
				if (p->d() == d()) p->rot();
				else rot();
			}
			rot();
		}
		update();
	}
};

int N;
ll D;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> D;

	return 0;
}
