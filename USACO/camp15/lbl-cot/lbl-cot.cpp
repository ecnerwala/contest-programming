#include<bits/stdc++.h> // andrew
using namespace std;
const int MAXN = 2e5;

struct node {
	node* c[2];
	int sum;
	node (int i) : sum(i) {} node(node* l, node* r) : c{l,r}, sum(l->sum + r->sum) {}
};

node* build(int l, int r) {
	node *cur = new node(0);
	//if(l == r) return node(0);
	if (l == r) return cur;
	else return new node(build(l, (l + r) / 2), build((l + r) / 2 + 1, r));
}
#define md ((l + r) >> 1)
node* insert(node* n, int l, int r, int x) {
	node *cur = new node(0);
	if(l == r) return new node(n->sum + 1);
	if (x <= md)  {
		return new node(insert(n->c[0], l, md, x), n->c[1]);
	}else return new node(n -> c[0], insert(n -> c[1], md + 1, r, x)); 
}


//void dfs(int x, int f)  {

vector<int> adj[MAXN];
node *ro[MAXN];
int par[MAXN][20];
int w[MAXN];

void dfs(int x, int f)  {
	par[x][0] = f;
	for (int i = 1; i < 20; i ++)
		par[x][i] = par[par[x][i - 1]][i - 1];
	ro[x] = insert(ro[f], 0, MAXN, w[x]);
	for (int i = 0; i < (int)adj[x].size() ; i ++)
		if(adj[x][i] != f) dfs(adj[x][i], x);
}
int depth(int x) {
	int ret = 0;
	for(int i = 19; i >= 0;i--) {
		if (par[x][i] != 0) x = par[x][i], ret += 1 << i;
	}
	return ret + 1;}

int dep[MAXN];

int lca(int x, int y)  {
	for(int i = 19; i >= 0; i--) 
	{if (dep[x] >= dep[y] + (1 << i)) x = par[x][i];
		if (dep[y] >= dep[x] + (1 << i)) y = par[y][i];}
	if(x == y) return x;
	for (int i = 19; i >= 0; i --) 
		if(par[x][i] != par[y][i]) x = par[x][i], y = par[y][i];
	return par[x][0];
}

int query(int x, int y, int k)  {
	if(!k) return dep[x] + dep[y] - 2 * dep[lca(x, y)] + 1;
	int p = lca(x, y), q = par[p][0];
	node *xx = ro[x], *yy = ro[y], *pp = ro[p], *qq = ro[q];
	int l = 0, r = MAXN;
	while (l != r)  {
		int tmp = xx -> c[0] -> sum + yy -> c[0] -> sum - pp -> c[0] -> sum - qq -> c[0] -> sum;
		if (k <= tmp) r = md; else l = md + 1;
		if(k<=tmp)xx=xx->c[0],yy=yy->c[0],pp=pp->c[0],qq=qq->c[0];
		else k -= tmp, xx=xx->c[1],yy=yy->c[1],pp=pp->c[1],qq=qq->c[1];
		
	}
	return l;
	
}

int main()  {

	int N; scanf("%d", &N);

	int M; scanf("%d", &M);
	vector<int> vals;
	for (int i = 0, x; i < N; i ++) scanf("%d", &x), vals.push_back(x);
	for(int i = 0; i < N; i++) w[i] = vals[i];
	sort(vals.begin(), vals.end());
	for(int i = N; i > 0; i--) w[i] = lower_bound(vals.begin(), vals.end(), w[i - 1]) - vals.begin();
	for (int i = 1; i < N ; i ++)  {
		int u, v; scanf("%d%d", &u, &v);
		adj[u].push_back(v), adj[v].push_back(u);
	} ro[0] = build(0, MAXN); dfs(1, 0);
	for(int i = 1; i <= N; i++) dep[i] = depth(i);

	while (M --)  {
		int u, v, k; scanf("%d%d%d", &u, &v, &k); printf("%d\n", k ? vals[query(u, v, k)] : query(u, v, k)); } 
	return 0;}


