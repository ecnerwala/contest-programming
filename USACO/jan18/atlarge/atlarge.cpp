#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e5;
int N;
const int MAXV = 2 * MAXN;
int V;
vector<int> adj[MAXV];
int P[MAXV];
int res[MAXV];
int D[MAXV];

// Pull up D's
void dfs1(int cur, int prv = -1) {
	P[cur] = prv;
	assert(!adj[cur].empty());
	if (adj[cur].size() == 1) {
		assert(adj[cur][0] == prv);
		D[cur] = 0;
		return;
	}
	D[cur] = V+1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs1(nxt, cur);
		D[cur] = min(D[cur], D[nxt] + 1);
	}
	assert(D[cur] <= V);
}
// Push down D's
void dfs2(int cur, int prv=-1) {
	P[cur] = prv;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		D[nxt] = min(D[nxt], D[cur] + 1);
		dfs2(nxt, cur);
	}
}

bool gone[MAXV];
int sz[MAXV];
void dfs_size(int cur, int prv = -1) {
	sz[cur] = 1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		if (gone[nxt]) continue;
		dfs_size(nxt, cur);
		sz[cur] += sz[nxt];
	}
}
int find_centroid(int cur) {
	dfs_size(cur);
	int s = sz[cur];
	if (s == 1) return cur;
	int prv = -1;
	while (true) {
		bool found = false;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			if (gone[nxt]) continue;
			if (sz[nxt] * 2 > s) {
				prv = cur;
				cur = nxt;
				found = true;
				break;
			}
		}
		if (!found) {
			return cur;
		}
	}
	assert(false);
}

unordered_map<int, map<int, int>> add_queue[MAXV];
void dfs3(int cur, int prv, int depth, map<int, int>::iterator it, map<int, int>::iterator end) {
	if (it == end) return;
	assert(it->first >= depth);
	if (it->first == depth) {
		res[cur] += it->second;
		it ++;
		if (it == end) return;
	}
	assert(it->first > depth);
	if (gone[cur]) {
		for (map<int, int>::iterator c = it; c != end; c ++) {
			add_queue[cur][prv][c->first - depth] += c->second;
		}
	} else {
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			dfs3(nxt, cur, depth+1, it, end);
		}
	}
}

void dfs_centroid(int st) {
	int cur = find_centroid(st);
	//cerr << "Centroid " << cur << '\n';
	gone[cur] = true;
	for (int nxt : adj[cur]) {
		if (gone[nxt]) continue;
		dfs_centroid(nxt);
	}
	map<int, int> mp;
	for (auto it : add_queue[cur]) {
		for (auto jt : it.second) {
			mp[jt.first] += jt.second;
			//cerr << it.first << ' ' << jt.first << ' ' << jt.second << '\n';
		}
	}
	if (D[cur] == 0) {
		res[cur] ++;
	} else {
		mp[D[cur]] ++;
	}
	for (int nxt : adj[cur]) {
		for (auto jt : add_queue[cur][nxt]) {
			mp[jt.first] -= jt.second;
		}
		if (D[nxt] == D[cur] - 1) {
			mp[D[cur]] --;
		}
		/*
		cerr << nxt << '\n';
		for (auto it : mp) {
			cerr << it.first << ' ' << it.second << '\n';
		}
		*/
		dfs3(nxt, cur, 1, mp.begin(), mp.end());
		if (D[nxt] == D[cur] - 1) {
			mp[D[cur]] ++;
		}
		for (auto jt : add_queue[cur][nxt]) {
			mp[jt.first] += jt.second;
		}
	}
	gone[cur] = false;
	//cerr << "Centroid " << cur << " done" << '\n';
}

void go() {
	assert(N >= 2);
	for (int i = 0; i < V; i++) {
		sort(adj[i].begin(), adj[i].end());
	}
	dfs1(N);
	dfs2(N);
	for (int i = 0; i < V; i++) {
		for (int j : adj[i]) {
			assert(D[i] % 2 != D[j] % 2);
			assert(abs(D[i] - D[j]) <= 1);
			assert(abs(D[i] - D[j]) == 1);
		}
	}
	//cerr << "DONE WITH D!\n";
	dfs_centroid(N);
}

int main() {
	if(fopen("atlarge.in", "r")) {
		freopen("atlarge.in", "r", stdin);
		freopen("atlarge.out", "w", stdout);
	}
	scanf("%d", &N);
	V = N + (N-1);
	for (int i = 0; i < N-1; i++) {
		int u, v; scanf("%d %d", &u, &v); u--, v--;
		adj[N + i].push_back(u);
		adj[N + i].push_back(v);
		adj[u].push_back(N+i);
		adj[v].push_back(N+i);
	}

	go();

	for (int i = 0; i < N; i++) {
		cout << res[i] << '\n';
	}

	return 0;
}
