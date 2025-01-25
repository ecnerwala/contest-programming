#include <bits/stdc++.h>
#pragma GCC optimize("Ofast,no-stack-protector")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,avx2,tune=native")

#define all(x) (x).begin(), (x).end()
#define itn int
#define make_unique(x) sort((x).begin(), (x).end()); (x).erase(unique((x).begin(), (x).end()), (x).end())

using namespace std;

inline int nxt() {
	int x;
	scanf("%d", &x);
	return x;
}

const int N = 111111;
int k[N];

inline int get(int l, int r) {
	int ans = 0;
	for (int i = l; i < r; ++i) {
		ans ^= k[i];
	}
	return ans;
}

void inc(int l, int r, int kk) {
	for (int i = l; i < r; ++i) {
		k[i] += kk;
	}
}

vector<int> a[N];
int sz[N];
int level[N];
int par[N];
int tin[N], tout[N];
int timer = 0;

void dfs(int v, int p = -1) {
	par[v] = p;
	auto it = find(all(a[v]), p);
	if (it != a[v].end()) {
		a[v].erase(it);
	}
	for (int x : a[v]) {
		level[x] = level[v] + 1;
		dfs(x, v);
		sz[v] += sz[x];
	}
	++sz[v];
	sort(all(a[v]), [&](int x, int y) {
		return sz[x] > sz[y];
	});
}

int w[N];

void dfsOrder(int v) {
	tin[v] = timer++;
	if (!a[v].empty()) {
		w[a[v][0]] = w[v];
	}
	for (int x : a[v]) {
		dfsOrder(x);
	}
	tout[v] = timer;
}

bool isPar(int u, int v) {
	return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int getAnswer(int u, int v) {
	int ans = 0;
	while (!isPar(w[u], v)) {
		ans ^= get(tin[w[u]], tin[u] + 1);
		u = par[w[u]];
	}
	while (w[v] != w[u]) {
		ans ^= get(tin[w[v]], tin[v] + 1);
		v = par[w[v]];
	}
	return ans ^ get(min(tin[u], tin[v]), max(tin[u], tin[v]) + 1);
}

void incAll(int u, int v, int kk) {
	while (!isPar(w[u], v)) {
		inc(tin[w[u]], tin[u] + 1, kk);
		u = par[w[u]];
	}
	while (w[v] != w[u]) {
		inc(tin[w[v]], tin[v] + 1, kk);
		v = par[w[v]];
	}
	inc(min(tin[u], tin[v]), max(tin[u], tin[v]) + 1, kk);
}

int main() {
	int n = nxt(), q = nxt();
	vector<int> curk(n);
	for (int i = 0; i < n; ++i) {
		curk[i] = i+1;
	}

	for (int i = 0; i < n - 1; ++i) {
		int u = nxt() - 1, v = nxt() - 1;
		a[u].push_back(v);
		a[v].push_back(u);
	}
	dfs(0);
	iota(w, w + n, 0);
	dfsOrder(0);
	for (int i = 0; i < n; ++i) {
		k[tin[i]] = curk[i];
	}

	while (q--) {
		char op; scanf(" %c", &op);
		int u = nxt() - 1, v = nxt() - 1;
		if (op == '+') {
			int x = nxt();
			incAll(u, v, x);
		} else if (op == '?'){
			printf("%d\n", getAnswer(u, v));
		} else assert(false);
	}
	return 0;
}
