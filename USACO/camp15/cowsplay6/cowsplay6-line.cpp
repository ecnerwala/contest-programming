//#define NDEBUG 
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
	//cerr << t->v << '\n';
	printt(t->c[!fp], fp);
	assert(t->s == 1 + (t->c[0] ? t->c[0]->s : 0) + (t->c[1] ? t->c[1]-> s : 0));
}

/*
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
	inline bool d() { return this == p->c[1]; }

	inline void update() { s = 1 + (c[0] ? c[0]->s : 0) + (c[1] ? c[1]->s : 0); }
	void propogate() {
		if(flip) {
			swap(c[0], c[1]);
			if(c[0]) c[0]->flip = true;
			if(c[1]) c[1]->flip = true;
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
				p->p->propogate();
				p->propogate();
				propogate();
				if(d() == p->d()) {
					p->rot();
				} else {
					rot();
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
		assert(!n || n->r());

		assert(!n || n->p == this);

		assert(t);
		assert(!n || n->t);

		////cerr << this << ' ' << this->c[0] << ' ' << this->c[1] << '\n';
		//cerr << (c[0] ? c[0]->s : 0) << ' ' << (c[1] ? c[1]->s : 0) << '\n';
		//cerr << this << ' ' << t->s << ' ' << s << '\n';

		if(c[1]) {
			node* v = c[1];
			c[1] = NULL;
			split(t, t, v->t, this->s - v->s);
			assert(v->s == v->t->s);

			update();
			assert(t->s == s);
		}

		//cerr << this << ' ' << this->c[0] << ' ' << this->c[1] << '\n';
		//cerr << (c[0] ? c[0]->s : 0) << ' ' << (c[1] ? c[1]->s : 0) << '\n';
		//cerr << this << ' ' << t->s << ' ' << s << '\n';

		if(n) {
			merge(t, t, n->t);
			n->t = NULL;
			c[1] = n;
			assert(c[1]->p == this);
			update();
		}

		//cerr << this << ' ' << this->c[0] << ' ' << this->c[1] << '\n';
		//cerr << (c[0] ? c[0]->s : 0) << ' ' << (c[1] ? c[1]->s : 0) << '\n';
		//cerr << this << ' ' << t->s << ' ' << s << '\n';

		assert(t->s == s);

	}

	// postcondition: propogated
	void expose() {
		//cerr << "hi\n";
		splay();
		//cerr << "hi\n";
		assert(!flip);
		make_child(NULL);
		while(p) {
			//cerr << p << ' ' << this << '\n';
			assert(r());
			p->splay();
			p->make_child(this);
			//cerr << "hi\n";
			assert(!t);
			assert(p->t);
			assert(!p->flip);
			assert(!flip);
			//cerr << p->s << '\n';
			//cerr << s << '\n';
			rot();
			update();
			//cerr << "hi\n";
			assert(r());
			//cerr << s << '\n';
		}
		assert(!p);
		assert(!c[1]);
	}

	// does not propogate
	void make_root() {
		//cerr << "hi\n";
		expose();
		assert(p == NULL);
		//cerr << "hi\n";
		assert(r());
		//cerr << "hi\n";
		flip = !flip;
		t->flip = !t->flip;
	}

} verts[MAXN];

vector<int> adj[MAXN];

node* dfs(int cur, int prv = -1) {
	node* res = &verts[cur];
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		dfs(nxt, cur)->p = res;
	}
	return res;
}
*/

tnode* ROOT = NULL;

pair<int, int> query(int a, int b) {
	bool swp = (a > b);
	if(swp) swap(a, b);
	assert(a <= b);
	tnode *l = NULL;
	tnode *r = NULL;
	assert(ROOT->s == N);
	assert(0 <= b + 1 && b + 1 <= N);
	split(ROOT, ROOT, r, b + 1);
	assert(ROOT->s == b + 1);
	assert(0 <= a && a <= N);
	split(ROOT, l, ROOT, a);
	assert(ROOT->s == b - a + 1);

	pair<int, int> v(get_left(ROOT), get_right(ROOT));
	ROOT->flip = !ROOT->flip;
	merge(ROOT, l, ROOT);
	merge(ROOT, ROOT, r);
	assert(ROOT->s == N);
	if(swp) swap(v.first, v.second);
	return v;
	/*
	//cerr << "query " << a << ' ' << b << '\n';
	//cerr << "making " << a << " root\n";
	verts[a].make_root();
	//cerr << "made " << a << " root\n";
	//cerr << verts[a].s << '\n';
	//cerr << "expose " << b << "\n";
	verts[b].expose();
	//cerr << "exposed " << b << "\n";
	tnode * t = verts[b].t;
	pair<int, int> res(get_left(t), get_right(t));
	assert(verts[b].t->s == verts[b].s);
	t->flip = !t->flip;
	//printt(t);
	return res;
	*/
}

int Q;
int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> Q;
	for(int i = 0; i < N; i++) {
		vals[i].v = i;
		vals[i].p = i;
		vals[i].c[0] = vals[i].c[1] = NULL;
		vals[i].flip = false;

		swap(vals[i].p, vals[rand() % (i + 1)].p); // random proirity

		vals[i].update();

		/*
		verts[i].p = NULL;
		verts[i].c[0] = verts[i].c[1] = NULL;
		verts[i].flip = false;

		verts[i].t = &vals[i];

		verts[i].update();
		*/
	}

	//cerr << vals << ' ' << verts << '\n';
	//cerr << sizeof(vals[0]) << ' ' << sizeof(verts[0]) << '\n';
	
	for(int i = 0; i < N; i++) {
		merge(ROOT, ROOT, &vals[i]);
	}

	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b;
		assert(a == b + 1 || a + 1 == b);
		/*
		adj[a].push_back(b);
		adj[b].push_back(a);
		*/
	}

	/*
	dfs(0);
	*/

	//cerr << "hi\n";

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
		va = res.first;
		vb = res.second;
		cout << va << ' ' << vb << '\n';
	}
	return 0;
}
