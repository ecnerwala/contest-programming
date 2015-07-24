#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N, M;
vector<int> adj[MAXN];

int depth[MAXN];
int lowval[MAXN];
int par[MAXN][25];
int cpar[MAXN];

int Q;

inline void NO() { cout << "No\n"; exit(0); }
inline void YES() { cout << "Yes\n"; exit(0); }

void dfs(int cur, int prv = 0) {
	par[cur][0] = prv;
	for(int i = 0; par[cur][i]; i++) par[cur][i + 1] = par[par[cur][i]][i];
	depth[cur] = depth[prv] + 1;
	lowval[cur] = depth[cur];
	cpar[cur] = prv;
	for(int nxt : adj[cur]) {
		if(nxt == prv) {
			prv = -1;
			continue;
		}
		if(!depth[nxt]) dfs(nxt, cur);
		lowval[cur] = min(lowval[cur], lowval[nxt]);
	}
	if(lowval[cur] == depth[cur]) {
		cpar[cur] = cur;
	}
}

int lca(int a, int b) {
	if(depth[a] < depth[b]) swap(a, b);
	for(int i = 0; depth[a] > depth[b]; i++) {
		if((depth[a] - depth[b]) & (1 << i)) a = par[a][i];
	}

	assert(depth[a] == depth[b]);

	if(a == b) return a;

	for(int i = 20; i >= 0; i--) {
		if(par[a][i] != par[b][i]) a = par[a][i], b = par[b][i];
	}
	assert(par[a][0] == par[b][0]);
	return par[a][0];
}

int getc(int a) { return cpar[a] == a ? a : cpar[a] = getc(cpar[a]); }

pair<bool, int> height[MAXN];

void verify(int cur) {
	for(int nxt : adj[cur]) {
		if(depth[nxt] <= depth[cur]) continue;
		verify(nxt);
	}

	if(cpar[cur] != cur) assert(height[cur].second == depth[cur]);

	assert(height[cur].second <= depth[cur]);
	if(height[cur].second < depth[cur]) {
		assert(cpar[cur] == cur);
		int p = cpar[par[cur][0]];
		assert(cpar[p] == p);
		assert(height[cur].second <= depth[p]);
		if(height[p].second < depth[p] && height[p].first != height[cur].first) NO();
		height[p].first = height[cur].first;
		height[p].second = min(height[p].second, height[cur].second);
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> Q;
	for(int i = 0; i < M; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	for(int i = 1; i <= N; i++) {
		if(!depth[i]) dfs(i);
	}
	for(int i = 1; i <= N; i++) getc(i);
	for(int i = 0; i < Q; i++) {
		int s, t; cin >> s >> t;
		s = cpar[s], t = cpar[t];
		int l = cpar[lca(s, t)];
		if(!l) NO();
		int d = depth[l];
		assert(depth[s] >= d && depth[t] >= d);
		if(depth[s] > d) {
			if(height[s].second && height[s].first != false) NO();
			height[s].first = false;
			height[s].second = max(depth[s] - d, height[s].second);
		}
		if(depth[t] > d) {
			if(height[t].second && height[t].first != true) NO();
			height[t].first = true;
			height[t].second = max(depth[t] - d, height[t].second);
		}
	}
	for(int i = 1; i <= N; i++) {
		height[i].second = depth[i] - height[i].second;
	}
	for(int i = 1; i <= N; i++) {
		if(depth[i] == 1) verify(i);
	}
	YES();
	return 0;
}
