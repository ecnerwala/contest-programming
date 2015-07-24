#include<bits/stdc++.h>
using namespace std;

struct node {
	node* c[2] = {0};
	int l = 0, r = 0, m = 0;
	int cnt = 0;
	
	node (int p) : l(p), r(p), m(p) { }

	node (node* a, node* b) : c{a,b}, l(a->l), r(b->r), m((l+r)/2), cnt(a->cnt + b->cnt) { }
};


void print(node* n) {
	if(n->l == n->r) {
		cerr << n->l << ' ' << n->cnt << '\n';
	} else {
		assert(n->c[0]);
		assert(n->c[1]);
		assert(n->l == n->c[0]->l);
		assert(n->m == n->c[0]->r);
		assert(n->m == n->c[1]->l - 1);
		assert(n->r == n->c[1]->r);

		assert(n->cnt == n->c[0]->cnt + n->c[1]->cnt);
		print(n->c[0]);
		print(n->c[1]);
	}
}

node* generate(int l, int r) {
	assert(l <= r);
	if(l == r) {
		return new node(l);
	} else {
		int m = (l + r) / 2;
		return new node(generate(l, m), generate(m + 1, r));
	}
}

node* insert(node* n, int v) {
	//cerr << n->l << ' ' << n->r << ' ' << v << '\n';
	assert(n->l <= v && v <= n->r);
	if(n->l == n->r) {
		node* res = new node(*n);
		res->cnt ++;
		return res;
	} else {
		if(v <= n->m) {
			return new node(insert(n->c[0], v), n->c[1]);
		} else {
			return new node(n->c[0], insert(n->c[1], v));
		}
	}
}

int get_kth(node* a, node* b, node* p, node* q, int k) {
	// all 3 vertices have the same structure
	assert(a && b && p && q);
	assert(a->l == p->l && a->r == p->r);
	assert(b->l == p->l && b->r == p->r);
	assert(b->l == q->l && b->r == q->r);

	assert(k > 0);
	assert(k <= a->cnt + b->cnt - p->cnt - q->cnt);

	if(p->l == p->r) {
		return p->l;
	} else {
		assert(a->c[0] && a->c[1]);
		assert(b->c[0] && b->c[1]);
		assert(p->c[0] && p->c[1]);
		assert(q->c[0] && q->c[1]);

		int v = a->c[0]->cnt + b->c[0]->cnt - p->c[0]->cnt - q->c[0]->cnt;
		if(k <= v) {
			return get_kth(a->c[0], b->c[0], p->c[0], q->c[0], k);
		} else {
			return get_kth(a->c[1], b->c[1], p->c[1], q->c[1], k - v);
		}
	}
}

const int MAXN = 2e5;
int N;
int nval[MAXN];
vector<int> adj[MAXN];

int V; // number of vals
int vals[MAXN];
int par[MAXN][30];
int depth[MAXN];
node* trees[MAXN];

int M;

// get the nth parent
int getpar(int a, int n) {
	for(int i = 0; n; i ++, n /= 2) {
		if(n & 1) a = par[a][i];
	}
	return a;
}

int lca(int a, int b) {
	if(depth[a] > depth[b]) {
		a = getpar(a, depth[a] - depth[b]);
	} else if (depth[b] > depth[a]){
		b = getpar(b, depth[b] - depth[a]);
	}
	assert(depth[a] == depth[b]);

	if(a == b) return a;

	assert(a != b);

	int i = 0;
	while(par[a][i] != par[b][i]) i++;

	assert(a != b);
	assert(par[a][i] == par[b][i]);

	assert(i < 30);

	for(i--; i >= 0; i--) {
		if(par[a][i] != par[b][i]) {
			a = par[a][i], b = par[b][i];
		}
		assert(a != b);
		assert(par[a][i] == par[b][i]);
	}
	assert(a != b);
	assert(par[a][0] == par[b][0]);
	return par[a][0];
}

void dfs(int cur, int prv = 0) {
	depth[cur] = depth[prv] + 1;

	par[cur][0] = prv;
	for(int i = 0; par[cur][i]; i++) {
		par[cur][i + 1] = par[par[cur][i]][i];
		assert(i + 1 < 30);
	}

	trees[cur] = insert(trees[prv], nval[cur]);

	assert(depth[cur] = trees[cur]->cnt);

	//cerr << cur << ' ' << prv << '\n'; print(trees[cur]); cerr << '\n';

	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		dfs(nxt, cur);
	}
}

int query(int u, int v, int k) {
	//assert(u != v);
	assert(0 < u && u <= N);
	assert(0 < v && v <= N);
	int p = lca(u, v);
	assert(p);
	int q = par[p][0];

	//cerr << p << ' ' << q << '\n';

	if(k == 0) {
		return depth[u] + depth[v] - depth[p] - depth[q];
	} else {
		return vals[get_kth(trees[u], trees[v], trees[p], trees[q], k)];
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 1; i <= N; i++) {
		cin >> nval[i];
		vals[i] = nval[i];
	}
	sort(vals, vals + N);
	V = int(unique(vals, vals + N) - vals);

	trees[0] = generate(0, V - 1);

	for(int i = 1; i <= N; i++) {
		nval[i] = int(lower_bound(vals, vals + V, nval[i]) - vals);
	}

	//for(int i = 1; i <= N; i++) cerr << nval[i] << ' '; cerr << '\n';

	for(int i = 0; i < N - 1; i++) {
		int u, v; cin >> u >> v;
		assert(u);
		assert(v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	dfs(1);

	for(int i = 0; i < M; i++) {
		int u, v, k; cin >> u >> v >> k;
		cout << query(u, v, k) << '\n';
	}
	return 0;
}
