#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1e5;
const int MAXM = 1e5;

int N;
int M;
int C[MAXM];
int T[MAXM];
pii E[MAXM];
unordered_multimap<int, int> edges;

bool used[MAXN];

inline void NO() { cout << "No\n"; exit(0); }

int adj[MAXN][2];
int deg[MAXN];

int vis[MAXN];

bool check_bipartite(int cur, int val = 1) {
	if(vis[cur]) {
		return vis[cur] == val;
	}
	vis[cur] = val;
	for(int i = 0; i < deg[cur]; i++) {
		if(!check_bipartite(adj[cur][i], -val)) return false;
	}
	return true;
}

vector<int> res;

void go() {
	for(int i = 0; i < M; i++) {
		edges.insert(make_pair(C[i], i));
	}

	for(auto it = edges.begin(); it != edges.end();) {
		int c = it->A;
		vector<int> v;
		while(it != edges.end() && it->A == c) {
			int e = it->B;
			v.push_back(E[e].A);
			v.push_back(E[e].B);
			if(deg[E[e].A] >= 2) NO();
			if(deg[E[e].B] >= 2) NO();
			adj[E[e].A][deg[E[e].A]++] = e;
			adj[E[e].B][deg[E[e].B]++] = e;
			it++;
		}
		sort(v.begin(), v.end());
		v.resize( unique(v.begin(), v.end()) - v.begin() );

		for(int cur : v) {
			assert(deg[cur]);
			if(deg[cur] == 2) {
				if(used[cur]) NO();
			}
		}

		for(int cur : v) vis[cur] = 0;

		for(int cur : v) {
			if(vis[cur]) continue;
			if(!check_bipartite(cur)) NO();
		}

		for(int cur : v) vis[cur] = 0;

		// now we have only even cycles and paths
		// deal with paths first
		for(int i : v) {
			if(deg[i] == 2) continue;
			int prv = 0;
			int cur = i;
			// deal with all this shit
			vector<int> verts;
			vector<int> edgs;
			do {
				int e = adj[cur][0] + adj[cur][1] - prv;
				int nxt = E[e].A + E[e].B - cur;
				vis[cur] = true;
				verts.push_back(cur);
				edgs.push_back(e);
				prv = e;
				cur = nxt;
			} while (deg[cur] == 2);
			vis[cur] = true;
			verts.push_back(cur);
			for(int j = 1; j + 1 < int(verts.size()); j++) {
				if(used[verts[j]]) NO();
				used[verts[j]] = true;
			}
		}

		for(int i : v) {
			if(vis[i]) continue;
			assert(deg[i] == 2);
			int t[2];
			t[0] = t[1] = 0;
			int cur = i;
			int prv = adj[i][0];
			do {
				assert(deg[cur] == 2);
				if(used[cur]) NO();
				used[cur] = true;
				int e = adj[cur][0] + adj[cur][1] - prv;
				int nxt = E[e].A + E[e].B - cur;
				t[0] = max(t[0], T[e]);
				prv = e;
				cur = nxt;
				swap(t[0], t[1]);
			} while(cur != i);
			cur = i;
			prv = adj[i][0];
			if(t[0] == t[1]) t[1] ++;
			do {
				assert(deg[cur] == 2);
				if(used[cur]) NO();
				used[cur] = true;
				int e = adj[cur][0] + adj[cur][1] - prv;
				int nxt = E[e].A + E[e].B - cur;
				if(t[0] < t[1]) res.push_back(e);
				prv = e;
				cur = nxt;
				swap(t[0], t[1]);
			} while(cur != i);
		}

		for(int cur : v) {
			// reset deg
			adj[cur][0] = adj[cur][1] = 0;
			deg[cur] = 0;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		cin >> E[i].A >> E[i].B >> C[i] >> T[i];
		E[i].A --, E[i].B --;
	}

	go();

	return 0;
}
