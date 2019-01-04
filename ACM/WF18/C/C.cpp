#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

struct node {
	node* p = nullptr;
	node* c[2] = {nullptr, nullptr};
	inline int d() { return this == p->c[1]; }

	ll val;
	ll cnt;
	ll lazy = 0;

	ll tot;

	node (ll v, ll c) : val(v), cnt(c) {
		update();
	}

	void propogate() {
		if (lazy) {
			val += lazy;
			if (c[0]) c[0]->lazy += lazy;
			if (c[1]) c[1]->lazy += lazy;
			lazy = 0;
		}
	}

	void update() {
		tot = cnt;
		if (c[0]) tot += c[0]->tot;
		if (c[1]) tot += c[1]->tot;
	}

	void rot() {
		assert(p);

		// must all be propogated
		p->propogate();
		this->propogate();

		int x = d();
		node* pa = p;
		node* ch = c[!x];

		if (pa->p) pa->p->c[pa->d()] = this;
		this->p = pa->p;

		pa->c[x] = this;
		if (ch) ch->p = pa;

		this->c[!x] = pa;
		pa->p = this;

		pa->update();
		this->update();
	}

	void propogate_all(node* root) {
		if (this == root) return;
		p->propogate_all(root);
		this->propogate();
	}

	void splay(node* root) {
		propogate_all(root);
		while (p != root) {
			if (p->p != root) {
				if (p->d() == this->d()) p->rot();
				else rot();
			}
			rot();
		}
	}
};

const int MAXN = 5e5;
int N;
vector<pair<int, ll>> adj[MAXN];

ll X[MAXN];
ll Y[MAXN];
ll TX, TY;

ll res = 0;

void dfs(ll cur, ll prv=-1) {
	for (auto it : adj[cur]) {
		int nxt = it.first; ll c = it.second;
		if (nxt == prv) continue;
		dfs(nxt, cur);
		// get the subtree sums
		X[cur] += X[nxt];
		Y[cur] += Y[nxt];

		ll xd = X[nxt], xu = TX - xd;
		ll yd = Y[nxt], yu = TY - yd;
		assert(xd + xu >= yd + yu);
		res += (max(yd - xd, 0ll) + max(yu - xu, 0ll)) * c;
	}
}

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N-1; i++) {
		int u, v; int c;
		scanf("%d %d %d", &u, &v, &c);
		u--, v--;
		adj[u].emplace_back(v, c);
		adj[v].emplace_back(u, c);
	}
	for (int i = 0; i < N; i++) {
		int x, y;
		scanf("%d %d", &x, &y);
		X[i] = x, Y[i] = y;
		TX += x, TY += y;
	}
	assert(TX >= TY);
	dfs(0);
	printf("%lld\n", res);
	return 0;
}
