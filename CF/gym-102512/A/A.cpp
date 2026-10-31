#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F& f) {
	int me = num[at] = ++Time, e, y, top = me;
	trav(pa, ed[at]) if (pa.second != par) {
		tie(y, e) = pa;
		if (num[y]) {
			top = min(top, num[y]);
			if (num[y] < me)
				st.push_back(e);
		} else {
			int si = sz(st);
			int up = dfs(y, e, f);
			top = min(top, up);
			if (up == me) {
				st.push_back(e);
				f(vi(st.begin() + si, st.end()));
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else {
				/* e is a bridge */
				f(vi({e}));
			}
		}
	}
	return top;
}

template<class F>
void bicomps(F f) {
	num.assign(sz(ed), 0);
	rep(i,0,sz(ed)) if (!num[i]) dfs(i, -1, f);
}

const int MAXV = 4.1e5;
int V;
vector<int> adj[MAXV];

int par[MAXV][20];
int depth[MAXV];

void dfsPar(int cur, int prv = -1) {
	memset(par[cur], -1, sizeof(par[cur]));
	depth[cur] = (prv == -1 ? 0 : depth[prv]) + 1;
	par[cur][0] = prv;
	for (int i = 0; par[cur][i] != -1; i++) {
		par[cur][i+1] = par[par[cur][i]][i];
	}
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfsPar(nxt, cur);
	}
}

int lca(int a, int b) {
	if (depth[b] < depth[a]) swap(a, b);
	for (int d = (depth[b] - depth[a]), i = 0; d; d /= 2, i++) {
		if (d & 1) b = par[b][i];
	}
	if (a == b) return a;
	int i = 0;
	while (par[a][i] != par[b][i]) i++;
	while (i) {
		--i;
		if (par[a][i] != par[b][i]) {
			a = par[a][i];
			b = par[b][i];
		}
	}
	return par[a][0];
}

int dist(int a, int b) {
	return depth[a] + depth[b] - 2 * depth[lca(a,b)];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	vector<pair<int, int>> edges(M);
	ed.resize(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		edges[e] = {u,v};
		ed[u].emplace_back(v, e);
		ed[v].emplace_back(u, e);
	}

	V = N;
	bicomps([&](const vector<int>& edgeList) {
		int curComp = V++;
		unordered_set<int> verts;
		for (auto e : edgeList) verts.insert(edges[e].first), verts.insert(edges[e].second);
		for (auto v : verts) {
			adj[curComp].push_back(v);
			adj[v].push_back(curComp);
		}
	});

	dfsPar(0);
	int Q; cin >> Q;
	while (Q--) {
		int a, b; cin >> a >> b; a--, b--;
		int d = dist(a, b);
		assert(d % 2 == 0);
		d /= 2;
		cout << d-1 << '\n';
	}

	return 0;
}
