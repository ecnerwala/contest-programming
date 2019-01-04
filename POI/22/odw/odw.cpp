#include <bits/stdc++.h>
using namespace std;

const int MAXN = 51000;
int N;
const int MAXK = 260;
const int K = 256;

int C[MAXN];
vector<int> adj[MAXN];
int depth[MAXN];
int par[MAXN][MAXK];
int sum[MAXN][MAXK];

void dfs(int cur, int prv = 0) {
	depth[cur] = depth[prv] + 1;
	par[cur][0] = cur;
	for (int i = 1; i <= K; i++) {
		par[cur][i] = par[prv][i-1];
		sum[cur][i] = sum[par[cur][i]][i] + C[cur];
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);
	}
}

int getanc(int a, int c) {
	while (c >= K) {
		a = par[a][K];
		c -= K;
	}
	a = par[a][c];
	return a;
}

int lca(int a, int b) {
	if (depth[a] < depth[b]) {
		swap(a,b);
	}
	assert(depth[a] >= depth[b]);
	if (depth[a] > depth[b]) {
		a = getanc(a, depth[a] - depth[b]);
	}
	assert(depth[a] == depth[b]);
	if (a == b) return a;
	assert(a != b);
	while (par[a][K] != par[b][K]) {
		a = par[a][K];
		b = par[b][K];
	}
	assert(a != b);
	assert(par[a][K] == par[b][K]);
	// ma is the same
	int ma = K;
	int mi = 0;
	while (ma - mi > 1) {
		int md = (ma + mi) / 2;
		if (par[a][md] == par[b][md]) {
			ma = md;
		} else {
			mi = md;
		}
	}
	assert(par[a][ma] == par[b][ma]);
	assert(par[a][mi] != par[b][mi]);
	return par[a][ma];
}

int queryPath(int a, int c, int k) {
	if (k > K) {
		int res = 0;
		assert(depth[a] >= depth[c]);
		while (depth[a] > depth[c]) {
			res += C[a];
			a = getanc(a, k);
		}
		return res;
	} else {
		int da = depth[a];
		int dc = depth[c];
		assert(dc <= da);
		int h = (da - dc + k - 1) / k * k; // min height that's above c
		assert(da - h <= dc);
		assert(da - (h - k) > dc);
		int nc = getanc(a, h);
		int res = sum[a][k] - sum[nc][k];
		return res;
	}
}

int query(int a, int b, int k) {
	int c = lca(a, b);
	int pc = par[c][1];
	// (a - c) + (b - pc)
	return queryPath(a, c, k) + queryPath(b, pc, k);
}

int T[MAXN];
int Q[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 1; i <= N; i++) {
		scanf("%d", &C[i]);
	}
	for (int i = 0; i < N-1; i++) {
		int a, b; scanf("%d %d", &a, &b);
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	dfs(1);
	for (int i = 0; i < N; i++) {
		scanf("%d", &T[i]);
	}
	for (int i = 0; i < N-1; i++) {
		scanf("%d", &Q[i]);
	}
	for (int i = 0; i < N-1; i++) {
		int res = query(T[i], T[i+1], Q[i]);
		printf("%d\n", res);
	}
	return 0;
}
