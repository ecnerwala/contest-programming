#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 200;

struct node {
	node* p;
	node* c[2];

	int sz;

	bool r() { return !(p && (this == p->c[d()])); }
	bool d() { return this == p->c[1]; }

	void rot() {
		assert(!r());
		int x = d();
		node* pa = p;
		node* ch = c[!x];

		pa->sz -= this->sz;
		this->sz -= ch ? ch->sz : 0;

		pa->sz += ch ? ch->sz : 0;
		this->sz += pa->sz;

		if(!pa->r()) pa->p->c[pa->d()] = this;
		p = pa->p;

		pa->c[x] = ch;
		if(ch) ch->p = pa;

		c[!x] = pa;
		pa->p = this;
	}

	void splay() {
		while(!r()) {
			if(!p->r()) {
				if(p->d() == d()) p->rot();
				else rot();
			}
			rot();
		}
	}

	void expose() {
		splay();
		c[1] = NULL;
		while(p) {
			p->splay();
			p->c[1] = this;
			rot();
		}
	}

	void cut() {
		expose();
		assert(c[0]);
		node* n = c[0];
		while(n->c[1]) {
			n = n->c[1];
		}
		n->splay();
		assert(n == p);
		n->sz -= this->sz;
		n->c[1] = NULL;
		this->p = NULL;
	}

	void link(node* n) {
		assert(n);
		n->expose();
		expose();
		assert(!c[0]);

		n->sz += this->sz;

		n->c[1] = this;
		this->p = n;
	}

	node() : p(NULL), c{NULL, NULL}, sz(1) { }
} V[MAXN];

int N;
int D;
int S[MAXN];
ll AS, CS, RS;
int M[MAXN];
ll AM, CM, RM;

const int MAXS = 1e6 + 200;
const int MAXD = 1e6 + 200;
const int MAXV = MAXS + MAXD;

vector<int> evt[MAXV];

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cin >> N;
		cin >> D;
		cin >> S[0] >> AS >> CS >> RS;
		cin >> M[0] >> AM >> CM >> RM;
		for(int i = 1; i < N; i++) {
			S[i] = int((AS * S[i - 1] + CS) % RS);
			M[i] = int((AM * M[i - 1] + CM) % RM);
		}

		for(int i = 1; i < N; i++) {
			M[i] %= i;
		}

		for(int i = 0; i < MAXV; i++) {
			evt[i].clear();
		}

		for(int i = 1; i < N; i++) {
			evt[S[i]].push_back(i);
			evt[S[i] + D + 1].push_back(i);
		}

		int res = 0;
		for(int s = 0; s < MAXV; s++) {
			for(int i : evt[s]) {
				if(S[i] == s) {
					//cerr << "link " << i << ' ' << M[i] << '\n';
					V[i].link(&V[M[i]]);
				} else if(s > S[i] + D) {
					//cerr << "cut  " << i << ' ' << M[i] << '\n';
					V[i].cut();
				}
			}
			if(s - D <= S[0] && S[0] <= s) {
				V[0].expose();
				//cerr << s << ' ' << V[0].sz << '\n';
				res = max(res, V[0].sz);
			}
		}
		cout << "Case #" << t << ": " << res << '\n';
	}
	return 0;
}
