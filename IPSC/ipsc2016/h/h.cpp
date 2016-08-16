#include<bits/stdc++.h>

void wssert(bool b) { if(!b) exit(0); }

using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

const int MAXN = 1e7 + 20;

int N;

// link-cut node
struct node {
	node* p;
	node* c[2];

	int s;
	ll sum;
	ll lazy;
	int dir;

	// isroot
	inline bool r() { return (p == NULL || !(this == p->c[0] || this == p->c[1])); }
	// direction
	inline bool d() { assert(!r()); return this == p->c[1]; }

	inline void update() {
		assert(!c[0] || c[0]->p == this);
		assert(!c[1] || c[1]->p == this);
		s = 1 + (c[0] ? c[0]->s : 0) + (c[1] ? c[1]->s : 0);
		if(dir == 2) {
			sum = (ll(lazy) + (c[0] ? c[0]->sum : 0) + (c[1] ? c[1]->sum : 0)) % MOD;
		} else {
			assert(abs(dir) <= 1);
			if(s & 1) sum = s * ll(lazy + (s - 1) / 2 * dir) % MOD;
			else sum = s / 2 * ll(2 * lazy + (s - 1) * dir) % MOD;
		}
	}
	void propogate() {
		assert(!c[0] || c[0]->p == this);
		assert(!c[1] || c[1]->p == this);
		if(dir != 2) {
			if(c[0]) {
				c[0]->lazy = lazy;
				c[0]->dir = dir;
				c[0]->update();
			}
			lazy += (c[0] ? c[0]->s : 0) * dir;
			lazy %= MOD;
			if(c[1]) {
				c[1]->lazy = lazy + dir;
				c[1]->dir = dir;
				c[1]->update();
			}
			dir = 2;
		}
	}

	// precondition: parent and current are propogated
	void rot() {
		assert(!c[0] || c[0]->p == this);
		assert(!c[1] || c[1]->p == this);
		assert(!r());

		int x = d();
		node* pa = p;
		node* ch = c[!x];

		assert((!ch) || ch->p == this);

		if(!pa->r()) pa->p->c[pa->d()] = this;
		this->p = pa->p;

		pa->c[x] = ch;
		if(ch) ch->p = pa;

		this->c[!x] = pa;
		pa->p = this;

		pa->update();
		update();
	}

	// postcondition: always propogated
	void splay() {
		if(r()) {
			propogate();
			return;
		}

		while(!r()) {
			if(!p->r()) {
				node* gp = p->p;
				node* pa = p;
				gp->propogate();
				pa->propogate();
				propogate();
				if(d() == p->d()) {
					pa->rot();
					assert(p == pa);
				} else {
					rot();
					assert(p == gp);
				}
				rot();
			} else {
				p->propogate();
				propogate();
				rot();
				assert(r());
			}
		}
		update();
	}

	// attach on right side
	// precondition: propogated
	void make_child(node* n) {
		assert(!c[0] || c[0]->p == this);
		assert(!c[1] || c[1]->p == this);
		assert(r());

		assert(dir == 2);

		if(c[1]) {
			node* v = c[1];
			c[1] = NULL;
			assert(v->r());

			update();
		}

		assert(!c[1]);

		if(n) {

			assert(n->r());
			assert(n->p == this);

			c[1] = n;
			assert(c[1]->p == this);

			update();
		}

		assert(!c[0] || c[0]->p == this);
		assert(!c[1] || c[1]->p == this);
	}

	// postcondition: propogated
	node* expose() {
		splay();
		make_child(NULL);
		node* res = this;
		while(p) {
			assert(r());
			res = p;
			p->splay();
			p->make_child(this);
			rot();
			update();
			assert(r());
		}
		assert(!p);
		assert(!c[1]);
		return res;
	}

};
node verts[MAXN];

inline int get_id(node* n) {
	return n ? int(n - verts) : 0;
}

vector<int> adj[MAXN];
int par[MAXN];
int depth[MAXN];

void dfs(int cur, int prv = -1) {
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		assert(1 <= nxt && nxt <= N);
		assert(1 <= cur && cur <= N);
		par[nxt] = cur;
		depth[nxt] = depth[cur] + 1;
		verts[nxt].p = &verts[cur];
		dfs(nxt, cur);
	}
}

ll T;

ll query(int a, int b, int c) {
	//cerr << "query " << a << ' ' << b << '\n';
	//cerr << "expose " << a << " root\n";
	verts[a].expose();
	//cerr << "expose " << b << "\n";
	node* lca = verts[b].expose();
	assert(lca);
	assert(lca == &(verts[b]) || lca == verts[b].c[0]);
	lca->expose();

	if(lca == verts + a) a = 0;
	else if(lca == verts + b) b = 0;
	if(a) verts[a].splay();
	if(b) verts[b].splay();
	assert(!a || verts[a].r());
	assert(!b || verts[b].r());

	ll start = (a ? verts[a].sum : 0) + (b ? verts[b].sum : 0);

	if(a) {
		T += verts[a].s * c;
		T %= MOD;

		verts[a].lazy = (T - c) % MOD;
		verts[a].dir = -c;
		verts[a].update();
	}

	if(b) {
		verts[b].lazy = (T) % MOD;
		verts[b].dir = c;
		verts[b].update();

		T += verts[b].s * c;
		T %= MOD;
	}

	ll end = (a ? verts[a].sum : 0) + (b ? verts[b].sum : 0);

	/*
	cerr << "query " << a << ' ' << b << ' ' << c << '\n';
	cerr << "lca " << get_id(lca) << "\n";
	for(int i = 1; i <= N; i++) { cerr << setw(2) << i << ' '; } cerr << '\n';
	for(int i = 1; i <= N; i++) { cerr << setw(2) << get_id(verts[i].p) << ' '; } cerr << '\n';
	for(int i = 1; i <= N; i++) { cerr << setw(2) << get_id(verts[i].c[0]) << ' '; } cerr << '\n';
	for(int i = 1; i <= N; i++) { cerr << setw(2) << get_id(verts[i].c[1]) << ' '; } cerr << '\n';
	*/

	//cerr << start << ' ' << end << '\n';

	return ((end - start) % MOD + MOD) % MOD;
}

int Q;

void go() {
	cin >> N >> Q;
	assert(N < MAXN);
	//if(N > 1000 || Q > 1000) assert(false);
	for(int i = 1; i <= N; i++) {
		adj[i].clear();

		verts[i].p = NULL;
		verts[i].c[0] = verts[i].c[1] = NULL;
		verts[i].dir = 2;
		verts[i].lazy = 0;

		verts[i].update();
	}

	//cerr << vals << ' ' << verts << '\n';
	//cerr << sizeof(vals[0]) << ' ' << sizeof(verts[0]) << '\n';

	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b;
		assert(1 <= a && a <= N);
		assert(1 <= b && b <= N);
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	dfs(1);

	T = 0;

	//for(int i = 0; i < N; i++) cerr << i << ' ' << par[i] << '\n';

	ll res = 0;
	for(int i = 0; i < Q; i++) {
		int a, b, c; cin >> a >> b >> c;

		ll v = query(a, b, c);
		//cerr << v << '\n';
		//assert(res == slow_query(a,b));
		res += v * (i + 1);
		res %= MOD;
		/*
		for(int k = 0; k < N; k++) {
			if(verts[k].r()) {
				assert(verts[k].s == verts[k].t->s);
			} else {
				assert(verts[k].p->c[verts[k].d()] == &verts[k]);
			}
		}
		*/
	}
	cout << res << '\n';
	cerr << res << '\n';

	for(int i = 1; i <= N; i++) {
		adj[i].clear();

		verts[i].p = NULL;
		verts[i].c[0] = verts[i].c[1] = NULL;
		verts[i].dir = 2;
		verts[i].lazy = 0;

		verts[i].update();
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int cases; cin >> cases;
	while(cases--) {
		cerr << cases << ':';
		go();
	}
	return 0;
}
