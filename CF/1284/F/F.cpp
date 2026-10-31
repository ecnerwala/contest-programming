#include<bits/stdc++.h>
using namespace std;

struct node {
	node * c[2] = {nullptr, nullptr};
	node * p = nullptr;
	bool flip = false;
 
	inline int d() {
		assert(p);
		return p->c[1] == this;
	}
 
	//whether or not root
	inline bool r() {
		return !(p && p->c[d()] == this);
	}
 
	int ownVal = 0;
	int firstVal = 0;
	int lastVal = 0;
	void update() {
		if (c[0] && c[0]->firstVal) {
			firstVal = c[0]->firstVal;
		} else if (ownVal) {
			firstVal = ownVal;
		} else if (c[1] && c[1]->firstVal) {
			firstVal = c[1]->firstVal;
		} else {
			firstVal = 0;
		}

		if (c[1] && c[1]->lastVal) {
			lastVal = c[1]->lastVal;
		} else if (ownVal) {
			lastVal = ownVal;
		} else if (c[0] && c[0]->lastVal) {
			lastVal = c[0]->lastVal;
		} else {
			lastVal = 0;
		}

		if (flip) {
			swap(firstVal, lastVal);
		}
	}
 
	void propogate() {
		if(flip) {
			flip = false;
			swap(c[0], c[1]);
			for(int i = 0; i < 2; i++) {
				if(c[i]) c[i]->flip = !c[i]->flip, swap(c[i]->firstVal, c[i]->lastVal);
			}
		}
	}
	void propogate_all() {
		if (!r()) p->propogate_all();
		propogate();
	}
 
	void rot() {
		assert(p);
		p->propogate();
		propogate();
 
		int x = d();
		node *pa = p;
		node *ch = c[!x];
 
		if(!pa->r()) pa->p->c[pa->d()] = this;
		p = pa->p;
 
		pa->c[x] = ch;
		if(ch) ch->p = pa;
 
		c[!x] = pa;
		pa->p = this;
 
		pa->update();
		update();
	}
 
	//post-condition - propogation done
	void splay() {
		propogate_all();
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
		while(p) {
			p->splay();
			p->c[1] = this;
			p->update();
			rot();
		}
		c[1] = NULL;
		update();
	}
 
	void make_root() {
		expose();
		flip = !flip;
		update();
	}
 
	void cut() {
		expose();
		assert(!flip);
		if(c[0]) c[0]->p = NULL;
		c[0] = NULL;
		update();
	}
 
	void link(node *n) {
		make_root();
		p = n;
	}
 
	node* getRoot() {
		if(p) return p->getRoot();
		else return this;
	}
};
 
const int MAXN = 3.1e5;
int N;
vector<pair<int, int>> adj[MAXN];
node n[MAXN * 2];

pair<int, int> E1[MAXN];
pair<int, int> E2[MAXN];
int ans[MAXN];

void dfs(int cur, int prv) {
	for (auto it : adj[cur]) {
		int nxt = it.first;
		if (nxt == prv) continue;
		dfs(nxt, cur);

		// delete this leaf
		n[cur].make_root();
		n[nxt].expose();
		int cutE = n[nxt].lastVal;
		assert(cutE);
		ans[it.second] = cutE;
		n[N+cutE].cut();
		n[N+cutE].ownVal = 0;
		n[N+cutE].update();
		n[N+cutE].link(&n[cur]);
	}
}
 
int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int e = 1; e <= N-1; e++) {
		int u, v; cin >> u >> v; E1[e] = {u, v};
		adj[u].push_back({v,e});
		adj[v].push_back({u,e});
	}
	for (int e = 1; e <= N-1; e++) {
		int u, v; cin >> u >> v; E2[e] = {u, v};
		n[N+e].ownVal = e; n[N+e].update();
		n[N+e].link(&n[u]);
		n[N+e].link(&n[v]);
	}

	dfs(1, 0);

	cout << N-1 << '\n';
	for (int e = 1; e <= N-1; e++) {
		//cerr << e << ' ' << ans[e] << '\n';
		cout << E1[e].first << ' ' << E1[e].second << ' ' << E2[ans[e]].first << ' ' << E2[ans[e]].second << '\n';
	}
	/*
	for(int i = 0; i < Q; i++) {
		string s; int a, b;
		cin >> s >> a >> b;
		if(s == "conn") {
			n[a].make_root();
			n[b].expose();
			n[a].splay();
			bool good = n[b].getRoot() == &(n[a]);
			cout << (good ? "YES" : "NO") << '\n';
		} else if(s == "add") {
			n[b].link(&(n[a]));
		} else {
			n[a].make_root();
			n[b].cut();
		}
	}
	*/
 
	return 0;
}
