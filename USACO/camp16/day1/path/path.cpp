#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
const int MAXQ = 2e5;

int N;
int M;
int Q;

int V[MAXQ][2];
int res[MAXQ];

vector<int> adj[MAXN];

int par[MAXN][20];
int depth[MAXN];

const int MAXS = 200;
int dist[MAXS][MAXS];
int S;
unordered_map<int, int> id;
int specials[MAXS];

void dfs(int cur, int prv = 0) {
	//cerr << cur << ' ' << prv << '\n';
	depth[cur] = depth[prv] + 1;
	par[cur][0] = prv;
	for(int i = 0; par[cur][i]; i++) {
		//cerr << cur << ' ' << i  << ' ' << par[cur][i] << '\n';
		par[cur][i + 1] = par[par[cur][i]][i];
	}
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		if(depth[nxt]) {

			if(!id.count(cur)) specials[S] = cur, id[cur] = S, S++;
			if(!id.count(nxt)) specials[S] = nxt, id[nxt] = S, S++;

			//cerr << cur << ' ' << nxt << '\n';
			dist[id[cur]][id[nxt]] = 1;
			continue;
		}
		dfs(nxt, cur);
	}
}

void reset() {
	for(int i = 0; i < MAXS; i++) {
		for(int j = 0; j < MAXS; j++) {
			dist[i][j] = ((i == j) ? 0 : N + 1);
		}
	}
	S = 0;
}

int lca(int a, int b) {
	//cerr << a << ' ' << b << '\n';
	if(depth[a] < depth[b]) swap(a, b);
	assert(depth[a] >= depth[b]);
	for(int i = 0; depth[a] > depth[b]; i++) {
		if((depth[a] - depth[b]) & (1 << i)) {
			a = par[a][i];
		}
	}
	assert(depth[a] == depth[b]);
	if(a == b) return a;
	assert(par[a][19] == par[b][19]);
	for(int i = 18; i >= 0; i--) {
		if(par[a][i] != par[b][i]) {
			a = par[a][i], b = par[b][i];
		}
		assert(par[a][i] == par[b][i]);
	}
	assert(par[a][0] == par[b][0]);
	return par[a][0];
}

int get_dist(int a, int b) {
	return depth[a] + depth[b] - 2 * depth[lca(a,b)];
}

void go() {
	cin >> N >> M >> Q;
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	for(int i = 0; i < Q; i++) {
		cin >> V[i][0] >> V[i][1];
	}

	reset();
	// now we do the actual stuff
	dfs(1);
	//for(int i = 0; i < S; i++) { cerr << specials[i] << '\n'; }

	if(N <= 50) {
		for(int i = 1; i <= N; i++) {
			if(!id.count(i)) {
				specials[S] = i, id[i] = S, S++;
			}
		}
	}

	for(int i = 0; i < S; i++) {
		for(int j = 0; j < S; j++) {
			dist[i][j] = min(dist[i][j], get_dist(specials[i], specials[j]));
		}
	}

	for(int k = 0; k < S; k++) {
		for(int i = 0; i < S; i++) {
			for(int j = 0; j < S; j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}

	for(int i = 0; i < Q; i++) {
		if(N <= 50) {
			assert(id.count(V[i][0]));
			assert(id.count(V[i][1]));
			res[i] = dist[id[V[i][0]]][id[V[i][1]]];
		} else {
			res[i] = get_dist(V[i][0], V[i][1]);
			for(int l = 0; l < S; l++) {
				for(int r = 0; r < S; r++) {
					res[i] = min(res[i], get_dist(specials[l], V[i][0]) + dist[l][r] + get_dist(specials[r], V[i][1]));
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	go();
	for(int i = 0; i < Q; i++) {
		cout << res[i] << '\n';
	}

	return 0;
}
