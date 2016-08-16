#define NDEBUG 
#include<bits/stdc++.h>

void wssert(bool b) { if(!b) exit(0); }

using namespace std;

const int MAXN = 2e5;

int N;

// let's use min-treap
struct tnode {
	tnode* c[2];
	int v;
	int p;
	int s;

	bool flip;

	void update() {
		s = 1 + (c[0] ? c[0]->s : 0) + (c[1] ? c[1]->s : 0);
	}
	void propogate() {
		if(flip) {
			swap(c[0], c[1]);
			if(c[0]) c[0]->flip = !c[0]->flip;
			if(c[1]) c[1]->flip = !c[1]->flip;
			flip = false;
		}
	}
} vals[MAXN];
void merge(tnode * &t, tnode * l, tnode * r) {
	if(!l) {
		t = r;
		return;
	}
	if(!r) {
		t = l;
		return;
	}

	assert(l && r);
	l->propogate();
	r->propogate();

	// priorities should never colide
	assert(l->p != r->p);
	assert(!l->flip);
	assert(!r->flip);

	if(l->p < r->p) {
		t = l;
		merge(l->c[1], l->c[1], r);
	} else {
		t = r;
		merge(r->c[0], l, r->c[0]);
	}
	t->update();
}
void split(tnode * n, tnode * &l, tnode * &r, int k) {
	if(!n) {
		l = r = NULL;
		return;
	}
	assert(n);
	n->propogate();

	assert(n->s == (n->c[0] ? n->c[0]->s : 0) + (n->c[1] ? n->c[1]->s : 0) + 1);
	assert(k >= 0 && k <= n->s);

	assert(!n->flip);
	int d = n->c[0] ? n->c[0]->s : 0;
	if(k <= d) {
		r = n;

		split(n->c[0], l, n->c[0], k);
		n->update();
	} else if(k == d + 1) {
		l = n;
		r = n->c[1];

		n->c[1] = NULL;
		n->update();
	} else {
		k -= d + 1;
		assert(n->c[1]);
		assert(k > 0);

		assert(k <= n->c[1]->s);
		
		l = n;

		split(n->c[1], n->c[1], r, k);
		n->update();
	}
}

// gets first and last of a tree
int get_left(tnode *t) {
	t->propogate();
	while(t->c[0]) t = t->c[0], t->propogate();
	return t->v;
}
int get_right(tnode *t) {
	t->propogate();
	while(t->c[1]) t = t->c[1], t->propogate();
	return t->v;
}

void printt(tnode *t, bool fp = false) {
	if(!t) return;
	if(t->flip) fp = !fp;
	printt(t->c[fp], fp);
	cerr << t->v << '\n';
	printt(t->c[!fp], fp);
	assert(t->s == 1 + (t->c[0] ? t->c[0]->s : 0) + (t->c[1] ? t->c[1]-> s : 0));
}

// link-cut node
struct node {
	node* p;
	node* c[2];

	int s;

	tnode * t;
	bool flip;

	// isroot
	inline bool r() { assert(bool(t) == (p == NULL || !(this == p->c[0] || this == p->c[1]))); if(t) assert(s == t->s); return t; }
	// direction
	inline bool d() { assert(!r()); return this == p->c[1]; }

	inline void update() { s = 1 + (c[0] ? c[0]->s : 0) + (c[1] ? c[1]->s : 0); }
	void propogate() {
		if(flip) {
			swap(c[0], c[1]);
			if(c[0]) c[0]->flip = !c[0]->flip;
			if(c[1]) c[1]->flip = !c[1]->flip;
			flip = false;
		}
	}

	// precondition: parent and current are propogated
	void rot() {
		assert(!r());
		assert(!t);

		int x = d();
		node* pa = p;
		node* ch = c[!x];

		assert(!pa->flip);
		assert(!flip);

		assert((!ch) || ch->p == this);

		if(!pa->r()) pa->p->c[pa->d()] = this;
		else this->t = pa->t, pa->t = NULL;
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
		assert(!flip);
		assert(r());

		assert(t);

		if(c[1]) {
			node* v = c[1];
			c[1] = NULL;
			split(t, t, v->t, this->s - v->s);
			assert(v->s == v->t->s);
			assert(v->r());

			update();
			assert(t->s == s);
		}

		assert(!flip);
		assert(!c[1]);

		if(n) {

			assert(n->r());
			assert(n->p == this);
			assert(n->t);

			merge(t, t, n->t);
			n->t = NULL;
			c[1] = n;
			assert(c[1]->p == this);

			update();
			assert(t->s == s);
		}

		assert(t->s == s);

	}

	// postcondition: propogated
	void expose() {
		splay();
		assert(!flip);
		make_child(NULL);
		while(p) {
			assert(r());
			p->splay();
			p->make_child(this);
			assert(!t);
			assert(p->t);
			assert(!p->flip);
			assert(!flip);
			rot();
			update();
			assert(r());
		}
		assert(!p);
		assert(!c[1]);
	}

	// does not propogate
	void make_root() {
		expose();
		assert(p == NULL);
		assert(r());
		flip = !flip;
		t->flip = !t->flip;
	}

} verts[MAXN];

vector<int> adj[MAXN];
int par[MAXN];
int depth[MAXN];

void dfs(int cur, int prv = -1) {
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		par[nxt] = cur;
		depth[nxt] = depth[cur] + 1;
		verts[nxt].p = &verts[cur];
		dfs(nxt, cur);
	}
}

pair<int, int> query(int a, int b) {
	//cerr << "query " << a << ' ' << b << '\n';
	//cerr << "making " << a << " root\n";
	verts[a].make_root();
	//cerr << "made " << a << " root\n";
	//cerr << "expose " << b << "\n";
	verts[b].expose();
	//cerr << "exposed " << b << "\n";
	tnode* t = verts[b].t;
	pair<int, int> res(get_left(t), get_right(t));
	assert(verts[b].t->s == verts[b].s);
	t->flip = !t->flip;
	//cerr << res.first << ' ' << res.second << '\n';
	return res;
}

int val[MAXN];
int slow_lca(int a, int b) {
	while(depth[a] > depth[b]) a = par[a];
	while(depth[b] > depth[a]) b = par[b];
	while(a != b) a = par[a], b = par[b];
	return a;
}
pair<int, int> slow_query(int a, int b) {
	//cerr << "slow " << a << ' ' << b << '\n';
	pair<int, int> res(val[a], val[b]);

	int c = slow_lca(a, b);
	vector<int> vec;
	for(int i = a; i != c; i = par[i]) {
		vec.push_back(val[i]);
	}
	vec.push_back(val[c]);
	int s = vec.size();
	for(int i = b; i != c; i = par[i]) {
		vec.push_back(val[i]);
	}
	for(int i = 0; s + i < int(vec.size()) - i - 1; i++) {
		swap(vec[s + i], vec[int(vec.size()) - i - 1]);
	}

	for(int i = a; i != c; i = par[i]) {
		assert(!vec.empty());
		val[i] = vec.back();
		vec.pop_back();
	}
	assert(!vec.empty());
	val[c] = vec.back();
	vec.pop_back();
	reverse(vec.begin(), vec.end());
	for(int i = b; i != c; i = par[i]) {
		assert(!vec.empty());
		val[i] = vec.back();
		vec.pop_back();
	}


	//cerr << res.first << ' ' << res.second << '\n';
	return res;
}

int Q;
int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> Q;
	//if(N > 1000 || Q > 1000) assert(false);
	for(int i = 0; i < N; i++) {
		vals[i].v = i;
		vals[i].p = i;
		vals[i].c[0] = vals[i].c[1] = NULL;
		vals[i].flip = false;

		swap(vals[i].p, vals[rand() % (i + 1)].p); // random proirity

		vals[i].update();

		verts[i].p = NULL;
		verts[i].c[0] = verts[i].c[1] = NULL;
		verts[i].flip = false;

		verts[i].t = &vals[i];

		verts[i].update();

		val[i] = i;
	}

	//cerr << vals << ' ' << verts << '\n';
	//cerr << sizeof(vals[0]) << ' ' << sizeof(verts[0]) << '\n';

	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	dfs(0);

	//for(int i = 0; i < N; i++) cerr << i << ' ' << par[i] << '\n';

	int va = 0, vb = 0;
	for(int i = 0; i < Q; i++) {
		int a, b; cin >> a >> b;
		a -= va;
		if(a < 0) a += N;
		assert(0 <= a && a < N);
		b -= vb;
		if(b < 0) b += N;
		assert(0 <= b && b < N);

		pair<int, int> res = query(a, b);
		//assert(res == slow_query(a,b));
		va = res.first;
		vb = res.second;
		cout << va << ' ' << vb << '\n';
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
	return 0;
}
