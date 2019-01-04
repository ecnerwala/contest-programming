#include<iostream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<deque>
#include<set>
#include<map>
#include<list>
#include<string>
#include<cstring>
#include<cassert>
#include<cmath>
#include<ctime>

using namespace std;

#define ITER(i, v) for(__typeof(v.begin()) i = v.begin(); i != v.end(); i++) 
#define A first
#define B second

typedef long long ll;
typedef pair<int, int> pii;

template <class T> void setmin(T &a, T b) { if(b < a) a = b; }
template <class T> void setmax(T &a, T b) { if(b > a) a = b; }

template <typename T, typename Comp = less<T> > class splay_tree {
	private:

		Comp comp;

		struct node {
			node *l, *r;
			node *p;

			T key;
			int size;

			node (T v) : l(NULL), r(NULL), p(NULL), key(v), size(1) {}

			~node()  {
				if(l) delete l;
				if(r) delete r;
			}

		} *root;

		void rot(node *c) {
			if(c == root) return;
			node *p = c->p;
			if(!p->p) {
				root = c;
			} else if(p == p->p->l) {
				p->p->l = c;
			} else {
				assert(p == p->p->r);
				p->p->r = c;
			}
			c->p = p->p;
			if(c == p->l) {
				p->l = c->r;
				if(p->l) p->l->p = p;
				c->r = p;
				c->r->p = c;
			} else {
				assert(c == p->r);
				p->r = c->l;
				if(p->r) p->r->p = p;
				c->l = p;
				c->l->p = c;
			}
			p->size = 1;
			if(p->l) p->size += p->l->size;
			if(p->r) p->size += p->r->size;
			c->size = 1;
			if(c->l) c->size += c->l->size;
			if(c->r) c->size += c->r->size;
		}

		void splay(node *c) {
			while(c != root) {
				node *p = c->p;
				if(p == root) {
					rot(c);
				} else {
					node *g = p->p;
					if(c == p->l && p == g->l || c == p->r && p == g->r) {
						rot(p);
						rot(c);
					} else {
						rot(c);
						rot(c);
					}
				}
			}
		}  

		node* subtree_minimum( node *u ) const {
			while( u->l ) u = u->l;
			return u;
		}

		node* subtree_maximum( node *u ) const {
			while( u->r ) u = u->r;
			return u;
		}

	public:

		node * next(node* c) const {
			if(c->r) {
				return subtree_minimum(c->r);
			}
			while(c->p) {
				if(c == c->p->l) {
					return c->p;
				} else {
					c = c->p;
				}
			}
			return NULL;
		}

		node * prev(node *c) const {
			if(c->l) {
				return subtree_maximum(c->l);
			}
			while(c->p) {
				if(c == c->p->r) {
					return c->p;
				} else {
					c = c->p;
				}
			}
			return NULL;
		}

		splay_tree() : root(NULL) {}

		splay_tree(const T &key) {
			root = new node(key);
		}

		int size (const node *n) const {
			return n ? n->size : 0;
		}
		int size () const { return size(root); }

		node* insert(const T &key) {
			//cerr << "inserting " << key << '\n';
			node *z = root;
			node *p = NULL;
			while(z) {
				p = z;
				if(comp(z->key, key) ) z = z->r;
				else z = z->l;
			}

			z = new node(key);
			z->p = p;

			if(!p) root = z;
			else if(comp(p->key, z->key)) p->r = z;
			else p->l = z;

			splay(z);

			return z;
		}

		node* find(const T &key) {
			node *z = root;
			while (z) {
				if (comp(z->key, key)) z = z->r;
				else if (comp(key, z->key)) z = z->l;
				else return z;
			}
			return 0;
		}

		node* minimum() const { return subtree_minimum(root); }
		node* maximum() const { return subtree_maximum(root); }

		bool empty() const { return !root; }

		//returns number of inversions
		ll merge(const splay_tree<T, Comp> &b) {
			ll ans = 0;
			ll tot = ll(size()) * b.size();

			for(node* n = b.minimum(); n; n = b.next(n)) {
				ans += size(insert(n->key)->r);
			}

			return min(ans, tot - ans);
		}

		~splay_tree () {
			if(root) delete root;
		}
};

typedef splay_tree<int> tr;

stack<tr*> sp;

int N;
stack<int> P;

ll go() {
	ll ans = 0;
	while(!P.empty()) {
		int v = P.top(); P.pop();
		//cerr << "Operation " << v << '\n';
		if(v) {
			sp.push(new tr(v));
		} else {
			tr *a, *b;
			a = sp.top(); sp.pop();
			b = sp.top(); sp.pop();
			if(a->size() < b->size()) swap(a, b);
			ans += a->merge(*b);
			sp.push(a);
			delete b;
		}
	}
	return ans;
}

int main() {
	scanf("%d", &N);

	for(int i = 0; i < 2 * N - 1; i ++) {
		int v; scanf("%d", &v);
		P.push(v);
	}

	//cerr << "input read\n";

	printf("%lld\n", go());
	return 0;
}
