#include<bits/stdc++.h>
using namespace std;

void wssert(bool b) { if(!b) exit(0); }

const int MAXN = 200000;
const int MAXQ = 200000;
int N, Q;
int A[MAXN];
vector<int> adj[MAXN];

int depth[MAXN];
int par[MAXN];
void dfs(int cur, int prv = -1) {
	A[cur] = cur;
	par[cur] = prv;
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		depth[nxt] = depth[cur] + 1;
		dfs(nxt, cur);
	}
}

int lca(int a, int b) {
	wssert(a != -1);
	wssert(b != -1);
	while(wssert(0 <= a && a < N && 0 <= b && b < N), depth[a] > depth[b]) {
		a = par[a];
		wssert(a != -1);
		wssert(depth[a] >= depth[b]);
	}
	while(wssert(0 <= a && a < N && 0 <= b && b < N), depth[b] > depth[a]) {
		b = par[b];
		wssert(b != -1);
		wssert(depth[b] >= depth[a]);
	}

	wssert(depth[b] == depth[a]);

	while(a != b) {
		a = par[a];
		b = par[b];
		wssert(a != -1);
		wssert(b != -1);
		wssert(depth[b] == depth[a]);
	}

	wssert(a == b);
	return a;
}

pair<int, int> query(int a, int b) {
	wssert(0 <= a && a < N);
	wssert(0 <= b && b < N);
	pair<int, int> res(A[a], A[b]);

	int c = lca(a, b);
	wssert(0 <= c && c < N);

	vector<int> v;

	for(int i = a; i != c; i = par[i]) {
		wssert(0 <= i && i < N);
		v.push_back(i);
	}

	v.push_back(c);


	int s = int(v.size());

	for(int i = b; i != c; i = par[i]) {
		wssert(0 <= i && i < N);
		v.push_back(i);
	}

	for(int i = 0; s + i < int(v.size()) - i - 1; i++) {
		swap(v[s + i], v[int(v.size()) - i - 1]);
	}

	for(int i = 0; i < int(v.size()) - i - 1; i++) {
		swap(A[v[i]], A[v[int(v.size()) - i - 1]]);
	}

	return res;
}

int main() {
	cin >> N >> Q;
	wssert(N <= 1000 && Q <= 1000);
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	depth[0] = 1;
	dfs(0);

	for(int i = 0; i < N; i++) {
		wssert(A[i] == i);
	}

	int va = 0, vb = 0;
	for(int q = 0; q < Q; q++) {
		int a, b; cin >> a >> b;
		a -= va;
		if(a < 0) a += N;
		b -= vb;
		if(b < 0) b += N;

		pair<int, int> res = query(a, b);
		va = res.first, vb = res.second;
		cout << va << ' ' << vb << '\n';
	}
	return 0;
}
